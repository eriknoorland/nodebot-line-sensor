const LineSensor = require('../dist/index.cjs');
const lineSensor = LineSensor('/dev/tty.usbserial-14130');

lineSensor.on('data', (data) => {
  console.log(data);
});

lineSensor.init();
