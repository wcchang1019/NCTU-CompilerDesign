int main(int argc, char **argv) {
    a = 0.000055454;
    a = 0123123.123123; 
    b = "aa \"bb \\cc";
    c = 01.23E4;
    c = 0001.23e4;
#pragma statistic off
    c = 00000.0000e-45;
#pragma token off //comments
#pragma source off /* comment
s */
#pragma statistic on
}