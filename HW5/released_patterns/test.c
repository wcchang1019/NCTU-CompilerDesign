int main()
{
	/*const int a = 3333333, b = 444444;
	const bool bb = true;
	const string s = "hello";
	int result = 11, xx = 15, c, d=11111;*/
	//c = a + b;
	//result = result + a + b;
	//print bb;
	//print s;
	/*int x;
	read x;
	if(x >100){
		if(x < 150){
			if(x < 125){
				print "x<125";
			}else{
				print "x>=125";
			}
		}else{
			print "x>=150";
		}
	}else{
		if(x > 50){
			print "x > 50";
		}else{
			print "x <= 50";
		}
	}

	int i;
	for(i=0;i<10;i=i+1){
		if(i % 2 == 1){
			print i;
		}
		int x = 0;
		while(x<3){
			if(x % 2 == 0){
				print "x:";
				print x;
				print "\n";
			}
			x = x + 1;
		}
	}*/
	int x = 0, y = 0, z = 0;
	for(x=0;x<5;x=x+1){
		for(y=0;y<4;y=y+1){
			for(z=0;z<3;z=z+1){
				print x;
				print " ";
				print y;
				print " ";
				print z;
				print "\n";
			}
		}
	}
	print "---------\n";
	x = 0;
	while(x < 5){
		y = 0;
		while(y < 4){
			z = 0;
			while(z < 5){
				print x;
				print " ";
				print y;
				print " ";
				print z;
				print "\n";
				z = z + 1;
			}
			y = y + 1;
		}
		x = x + 1;
	}
	x = 0;
	while(x < 10){
		print x;
		print "\n";
		x = x + 1;
	}

	return 0;
}
/*
int testtset(int a, int b, double xxx){


}
int test2(){

	
}


int test3(int c, int d){
	return c+d;
}
void test4(){

	
}*/