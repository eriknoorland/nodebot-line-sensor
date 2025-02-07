export type DataPacket = [number, number, number, number, number, number, number, number, number, number, number, number, number, number, number, number];

export type Data = number[];

export type LineSensorEvents = {
  'data': Data[]
  'error': Error[]
  'disconnect': []
  'close': []
}

type EventName = keyof LineSensorEvents
type EventHandler = (...eventArg: LineSensorEvents[EventName]) => void

export interface LineSensor {
  close: () => Promise<void>
  init: () => Promise<void>
  isReady: () => Promise<void>  
  on: (eventName: EventName, handler: EventHandler) => void
  off: (eventName: EventName, handler: EventHandler) => void
}