.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.method public static main([Ljava/lang/String;)V
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic output/_sc Ljava/util/Scanner;
.limit stack 100
.limit locals 100
ldc 0
istore 1
ldc 0
istore 2
ldc 0
istore 3
ldc 0
istore 1
Fbegin_0:
iload 1
ldc 5
isub
iflt Ltrue_0
iconst_0
goto Lfalse_0
Ltrue_0:
iconst_1
Lfalse_0:
ifeq ForC_0
ldc 0
istore 2
Fbegin_1:
iload 2
ldc 4
isub
iflt Ltrue_1
iconst_0
goto Lfalse_1
Ltrue_1:
iconst_1
Lfalse_1:
ifeq ForC_1
ldc 0
istore 3
Fbegin_2:
iload 3
ldc 3
isub
iflt Ltrue_2
iconst_0
goto Lfalse_2
Ltrue_2:
iconst_1
Lfalse_2:
ifeq ForC_2
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/print(I)V
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 2
invokevirtual java/io/PrintStream/print(I)V
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 3
invokevirtual java/io/PrintStream/print(I)V
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "\n"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
iload 3
ldc 1
iadd
istore 3
goto Fbegin_2
ForC_2:
iload 2
ldc 1
iadd
istore 2
goto Fbegin_1
ForC_1:
iload 1
ldc 1
iadd
istore 1
goto Fbegin_0
ForC_0:
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "---------
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
ldc 0
istore 1
wbegin_0:
iload 1
ldc 5
isub
iflt Ltrue_3
iconst_0
goto Lfalse_3
Ltrue_3:
iconst_1
Lfalse_3:
ifeq WhileC_0
ldc 0
istore 2
wbegin_1:
iload 2
ldc 4
isub
iflt Ltrue_4
iconst_0
goto Lfalse_4
Ltrue_4:
iconst_1
Lfalse_4:
ifeq WhileC_1
ldc 0
istore 3
wbegin_2:
iload 3
ldc 5
isub
iflt Ltrue_5
iconst_0
goto Lfalse_5
Ltrue_5:
iconst_1
Lfalse_5:
ifeq WhileC_2
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/print(I)V
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 2
invokevirtual java/io/PrintStream/print(I)V
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 3
invokevirtual java/io/PrintStream/print(I)V
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "\n"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
iload 3
ldc 1
iadd
istore 3
goto wbegin_2
WhileC_2:
iload 2
ldc 1
iadd
istore 2
goto wbegin_1
WhileC_1:
iload 1
ldc 1
iadd
istore 1
goto wbegin_0
WhileC_0:
ldc 0
istore 1
wbegin_3:
iload 1
ldc 10
isub
iflt Ltrue_6
iconst_0
goto Lfalse_6
Ltrue_6:
iconst_1
Lfalse_6:
ifeq WhileC_3
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/print(I)V
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "\n"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
iload 1
ldc 1
iadd
istore 1
goto wbegin_3
WhileC_3:
ldc 0
return
.end method
