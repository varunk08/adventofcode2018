import collections

class Facility:
    def __init__(self):
        self.min_steps = collections.defaultdict(lambda : float('inf'))
        self.min_path = collections.defaultdict(list)

    def step(self, directions, save_path = False):
        branches = {}
        branch = n = x = y = 0
        branched = False
        path = [(0, 0)]
        while True:
            if branched:
                if save_path:
                    x, y, n, path = branches[branch]
                else:
                    x, y, n = branches[branch]
                branched = False
            if save_path:
                if path[-1] != (x, y):
                    path.append((x, y))
                if n < self.min_steps[(x, y)]:
                    self.min_path[(x, y)] = path[:]
            n = self.min_steps[(x, y)] = min(self.min_steps[(x, y)], n)
            c = next(directions)
            if c == '$':
                return
            if c == '(':
                branch += 1
                if save_path:
                    branches[branch] = (x, y, n, path[:])
                else:
                    branches[branch] = (x, y, n)
            if c == ')':
                branch -= 1
            if c == '|':
                branched = True
            if c == 'N':
                y += 1
            if c == 'S':
                y -= 1
            if c == 'E':
                x += 1
            if c == 'W':
                x -= 1
            n += 1


if __name__ == '__main__':
    facility = Facility()
    with open('day20Input.txt', 'r') as f:
        facility.step(iter(f.read()))
    print(max(facility.min_steps.items(), key = lambda x : x[1])[1])
print(len([k for k, v in facility.min_steps.items() if v >= 1000]))
print(facility.min_steps.values())