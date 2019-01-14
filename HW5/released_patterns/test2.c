// test.c
int a, b;
bool d;
int foo( int a ) {
int i, result;
result = 0;
i = 1;
while( i <= a ) {
result = result + i;
i = i + 1;
}
return result;
}
int main() {
int c ;
read a;
c = foo( a );
print c;
print "\n";
if( c >= 100 ) {
print "c >= 100 \n";
}
else {
print "c < 100 \n";
}
}