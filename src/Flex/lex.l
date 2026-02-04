//这一段是把c代码直接拷贝到lex中，里面可以放注释
%{
/*
*dfsdfsdfsdf
*/

}%
%%

//正则表达式模式匹配,每一行由两部分组成 pattern 和action, 中间是一个空格,
//pattern为正则表达式 ， action是一段代码, 其中| 表示一个特殊的action，它和后面一条语句的action相同
[\t ]+ ;   //表示识别 tab或者 空格 1次或多次
is |          //识别is 
am |
are |
go { printf("%s:is a verb\n",yytext);}  //行为代码段中可以有return, yylex()碰到return就会return;否则会执行到结束
.|\n {ECHO;}
%%

main()
{
	yylex();
}


