#include <mgos.h>
#include "mgos_http_server.h"

static struct mg_serve_http_opts s_http_server_opts;

static void handle_gzip(struct mg_connection *nc, struct http_message* msg,
                        struct mg_serve_http_opts* opts) {
  struct mg_str uri = mg_mk_str_n(msg->uri.p, msg->uri.len);
  bool gzip = strncmp(uri.p + uri.len - 3, ".gz", 3) == 0;
  if (gzip) {
    opts->extra_headers = "Content-Encoding: gzip";
  }
  (void)(nc);
}

static void http_msg_print(const struct http_message* msg) {
  LOG(LL_INFO, ("     message: \"%.*s\"\n", msg->message.len, msg->message.p));
  LOG(LL_INFO, ("      method: \"%.*s\"\n", msg->method.len, msg->method.p));
  LOG(LL_INFO, ("         uri: \"%.*s\"\n", msg->uri.len, msg->uri.p));
}

static void root_handler(struct mg_connection *nc, int ev, void *p, void *user_data) {
  (void) user_data;
  if (ev != MG_EV_HTTP_REQUEST) {
    return;
  }
  LOG(LL_INFO, ("root_handler"));
  struct http_message* msg = (struct http_message*) (p);
  http_msg_print(msg);

  struct mg_serve_http_opts opts;
  memcpy(&opts, &s_http_server_opts, sizeof (opts));
  handle_gzip(nc, msg, &opts);
  mg_serve_http(nc, msg, opts);
}

enum mgos_app_init_result mgos_app_init(void) {
  memset(&s_http_server_opts, 0, sizeof (s_http_server_opts));
  s_http_server_opts.document_root = mgos_sys_config_get_http_document_root();

  s_http_server_opts.custom_mime_types =
    ".js.gz=application/javascript; charset=utf-8,.css.gz=text/css; charset=utf-8";

  mgos_register_http_endpoint("/", root_handler, NULL);

  return MGOS_APP_INIT_SUCCESS;
}
