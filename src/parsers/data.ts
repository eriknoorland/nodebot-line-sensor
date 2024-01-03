import { math } from '@eriknoorland/nodebot-utils';
import { Data } from '../interfaces';
import { DataPacket } from '../types';

export default (data: DataPacket): Data => {
  return data.reduce((acc, value, index, array) => {
    if (index % 2 === 1) {
      const msB = math.parseDecToBinary(data[index - 1]);
      const lsB = math.parseDecToBinary(data[index]);

      return [
        ...acc,
        parseInt(`${msB}${lsB}`, 2),
      ];
    }

    return acc;
  }, []);
};
