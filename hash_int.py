HASH_TABLE_LENGTH = 1000

class HashIntSet:
    def __init__(self):
        self.table = []
        for i in range(HASH_TABLE_LENGTH):
            self.table.append([])

    def _cellForInt(self, num):
        pos = num % HASH_TABLE_LENGTH
        return self.table[pos]

    def insert(self, num):
        if self.hasInt(num):
            return
        cell = self._cellForInt(num)
        cell.append(num)

    def delete(self, num):
        cell = self._cellForInt(num)
        for val in cell:
            if val == num:
                cell.remove(val)
                return

    def hasInt(self, num):
        cell = self._cellForInt(num)
        for val in cell:
            if val == num:
                return True
        return False

def test():
    a = HashIntSet()
    a.insert(2000)
    a.insert(18399)
    a.insert(289384)
    if a.hasInt(2000):
        print "Ok: Has 2000"
    else:
        print "Error: Not has 2000"
    if a.hasInt(4929):
        print "Error: Has 4929"
    else:
        print "Ok: Not has 4929"

test()
