import { math } from '@eriknoorland/nodebot-utils';
import { DataPacket, Data } from '../interfaces';

export default (data: DataPacket): Data => {
  const mostSignificantBits = data.filter((value, index) => index % 2 === 0)
  const leastSignificantBits = data.filter((value, index) => index % 2 === 1)
  
  return leastSignificantBits.map((value, index) => {
    const msB = math.parseDecToBinary(mostSignificantBits[index]);
    const lsB = math.parseDecToBinary(value);

    return parseInt(`${msB}${lsB}`, 2);
  })
};
