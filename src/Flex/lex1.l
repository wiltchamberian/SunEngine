%{

	enum {
	   LOOKUP = 0,
	   VERB,
	   ADJ,
	   ADV,
	   NOUN,
	   REP,
	   PRON,
	   CONJ
	};
	
	int state;
	int add_word(int type, char*word);
	int lookup_word(char* word);

}%
%%

\n {state=LOOKUP;}
^verb {state=VERB;}
^adj  {state = ADJ;}
^adv  {state=ADV;}
^noun {state=NOUN;}
^prep {state=PREP;}
^pron {state=PRON;}
^conj {state=CONJ;}

[a-zA-Z]+ {
	if(state!=LOOKUP){
		add_word(state,yytext);
	}else{
		switch(lookup_word(yytext)){
			case VERB: printf("%s:\n",yytext);break;
			case ADJ: printf("%s\n",yytext);break;
			default:
				printf("%s,don't recognize\n",yytext);break;
		}
	}
}
. ;
%%
main(){
	yylex();
}

