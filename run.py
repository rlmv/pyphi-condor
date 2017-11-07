from caller import call_quack, out, start_mw

if __name__ == '__main__':
    call_quack()
    out()
    start_mw(b"test\0another")
    print('MIDDLE')
    print('*' * 100)
#    start_mw()
