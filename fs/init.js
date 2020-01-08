load("api_rpc.js");
load("api_sys.js");
load("api_gpio.js");

let led = 2;
let blinkLed = 13;
let state = true;
let blinkState = true;

let setLED = function() {
  GPIO.write(led, state);
  state = !state;
  print("LED on ->", state);
};

let setBLINK = function() {
  GPIO.write(blinkLed, blinkState);
  blinkState = !blinkState;
  print("LED on ->", blinkState);
};

GPIO.set_mode(led, GPIO.MODE_OUTPUT);
GPIO.set_mode(blinkLed, GPIO.MODE_OUTPUT);

RPC.addHandler("onOff", function(args) {
  setLED();
  return {
    status: "OK"
  };
});

RPC.addHandler("blink", function(args) {
  setBLINK();
  return {
    status: "OK"
  };
});
