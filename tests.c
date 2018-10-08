//File for writing tests

void logError(char* type){
    FILE *log = fopen("errLog.txt", "w");
    if (f == NULL){
        printf(stderr, "File could not be opened.");
        return -1;
    }
    fprintf(log, "%s", type);
    return;
}

int main {



    return 0;
}