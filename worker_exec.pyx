from caller cimport start_worker

import sys

if __name__ == "__main__":
    start_worker(len(sys.argv), sys.argv)
