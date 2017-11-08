from caller import call_quack, out, start_mw

if __name__ == '__main__':
    call_quack()
    out()

    class A:
        def __init__(self, x):
            self.x = x

        def __str__(self):
            return "My name is {}, my x is {}".format(type(self), self.x)

    start_mw(A(1))
    print('MIDDLE')
    print('*' * 100)
#    start_mw()
