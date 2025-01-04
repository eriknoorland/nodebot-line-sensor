import assert from 'assert'
import { DataPacket } from '../src/interfaces'
import dataParser from '../src/parsers/data'

const data: DataPacket = [0x00, 0x0A, 0x00, 0x14, 0x00, 0x1E, 0x00, 0x28, 0x00, 0x32, 0x00, 0x3C, 0x00, 0x46, 0x00, 0x50]

describe('data parser', () => {
  it('should return an array of numbers half the length of the given data', () => {
    const actualResult = dataParser(data)

    assert.equal(actualResult.length, data.length / 2);
  })

  it('should return an array of numbers', () => {
    const expectedResult = [10, 20, 30, 40, 50, 60, 70, 80]
    const actualResult = dataParser(data)

    assert.deepStrictEqual(actualResult, expectedResult);
  })
})