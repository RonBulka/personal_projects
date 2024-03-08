#!/bin/bash

make
time ./firewall.sh test0-rules.in < test0-pkts.in > test0.out
time ./firewall.sh test1-rules.in < test1-pkts.in > test1.out
time ./firewall.sh test2-rules.in < test2-pkts.in > test2.out
time ./firewall.sh test3-rules.in < test3-pkts.in > test3.out
diff test0.out test0-pkts.cmp
diff test1.out test1-pkts.cmp
diff test2.out test2-pkts.cmp
diff test3.out test3-pkts.cmp

# valgrind --leak-check=full --show-leak-kinds=all ./firewall.sh test0-rules.in < test0-pkts.in > test0.out
# valgrind --leak-check=full --show-leak-kinds=all ./firewall.exe src-port=123-123 < test1-pkts.in > test1.out