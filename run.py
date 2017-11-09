from caller import start_mw, mw_print

import pyphi

if __name__ == '__main__':

    s = pyphi.examples.basic_subsystem()

    class A:
        def __init__(self, x):
            self.x = x

        def __str__(self):
            return "My name is {}, my x is {}".format(type(self), self.x)

        def run(self, y):
            return self.x.concept(y)

    cand = list(pyphi.utils.powerset((0, 1, 2)))
    start_mw(A(s), cand)
    # print('MIDDLE')
    # print('*' * 100)
    # start_mw(A(2))
