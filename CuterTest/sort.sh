#/bin/sh
mkdir -p Result
cat *equ.free* > Result/equ.free.list
cat *gencon* > Result/gencon.list
cat *equ*    > Result/equ.list
cat *ineq*   > Result/ineq.list
cat *unc*    > Result/unc.list
cat *box*    > Result/box.list
cat *free*   > Result/free.list
cat *upper*  > Result/upper.list
cat *lower*  > Result/lower.list
cat *linear* > Result/linear.list
cat *nonlin* > Result/nonlin.list
