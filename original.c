nextChar(c);
                 								while (c != '\n' && c != EOF)
                 								   nextChar(c);                 								
                  									if (c == EOF){
								                	    token->Type = tokenEndOfFile;
								                    	scanRet(kwstring, LEXICAL);
                								}
             							    }
               								while (c != '='){
								                if (c == EOF){
								                    token->Type = tokenEndOfFile;
								                    scanRet(kwstring, LEXICAL);
								                }
                								nextChar(c);
                							}
                							//nextChar(c);
											//	if(c == '='){
											//		nextChar(c);








        if (c == '\n'){
          nextChar(c);
          if ((c == '=')){
            nextChar(c);
            if (c == 'b'){
              nextChar(c);
              if(c == 'e'){
                nextChar(c);
                if(c == 'g'){
                  nextChar(c);
                  if(c == 'i'){
                    nextChar(c);
                    if(c == 'n'){
                      nextChar(c);
                              if (c ==  '\n') {   
                                nextChar(c);                              
                        if(c == '='){
                                  nextChar(c);                              
                          if(c == 'e'){
                            nextChar(c);
                            if (c == 'n'){
                              nextChar(c);
                              if (c == 'd'){
                                nextChar(c);
                                if(c == '\n'){
                                  token -> Type = tokenEndOfLine;
                                  state = stateStart;
          }}}}}}}}}}}}
          else{
            returnChar(c, code);
            token -> Type = tokenEndOfLine;
            }
        }