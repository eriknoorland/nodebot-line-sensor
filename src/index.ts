import EventEmitter from 'events';
import { SerialPort } from 'serialport';
import Parser from './Parser';

const cobs = require('cobs');

export default (path: string) => {
  const eventEmitter = new EventEmitter();
  const requestStartFlag = 0xA1;

  let port: SerialPort;
  let parser;

  function init(): Promise<void> {
    return new Promise((resolve, reject) => {
      if (port) {
        setTimeout(reject, 0);
      }

      port = new SerialPort({ path, baudRate: 115200 });
      parser = new Parser();

      port.pipe(parser);

      port.on('error', error => eventEmitter.emit('error', error));
      port.on('disconnect', () => eventEmitter.emit('disconnect'));
      port.on('close', () => eventEmitter.emit('close'));
      port.on('open', onPortOpen);

      parser.on('ready', resolve);
      parser.on('data', data => eventEmitter.emit('data', data));
    });
  }

  function isReady() {
    writeToSerialPort([requestStartFlag, 0x01]);

    return Promise.resolve();
  }

  function close(): Promise<void> {
    return new Promise(resolve => {
      writeToSerialPort([requestStartFlag, 0x02]);

      port.close(error => {
        resolve();
      });
    });
  }

  function writeToSerialPort(data: number[]) {
    port.write(cobs.encode(Buffer.from(data), true));
  }

  function onPortOpen() {
    port.flush(error => {
      if (error) {
        eventEmitter.emit('error', error);
      }
    });
  }

  return Object.freeze({
    close,
    init,
    isReady,
    on: eventEmitter.on.bind(eventEmitter),
    off: eventEmitter.off.bind(eventEmitter),
  });
};
