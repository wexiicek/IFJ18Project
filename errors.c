returnError(int value, char[] errorDescription){
    switch(value){
        case 1:
            printf(stderr, "An error occured during lexical analysis.");
            return 1;
            break;
        case 2:
            printf(stderr, "Incorect syntax: %s.   ", errorDescription);
            return 2;
            break;
        case 3:
            printf(stderr, "An error occured: %s. Shutting down.", errorDescription);
            return 3;
            break;
        case 4:

            return 4;
            break;
        case 6:
            
            return 6;
            break;
        case 99:

            return 99;
            break;
    }
}