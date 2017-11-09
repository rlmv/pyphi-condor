import sys

from caller import start_worker


if __name__ == "__main__":
    start_worker(len(sys.argv), sys.argv)
