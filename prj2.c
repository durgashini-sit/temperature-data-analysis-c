
#include <stdio.h>
#include <math.h>


//5040 for all data. 56 for 1 day, 1680 for 1 mth
#define max 5040

//#ifdef prj 2
void hourave(float readings[max],float month[30][16],int a){

     float sum;
     int first_data,hr_data,day,hour;

    for(day=0;day<30;day++){
        for(hour=0;hour<14;hour++){
            first_data=(56*day+a)+4*hour;
            sum=0;

            for(hr_data=0;hr_data<4;hr_data++){
                sum=sum+readings[first_data+hr_data];
                }

            month[day][hour]=sum/4;
            }
    }
}

void dailymsd(float readings[max],float m[30][16],int a){

     float sum;
     int first_data,hr_data,day,hour;

     //mean

     for(day=0;day<30;day++){
            first_data=((56*day)+a);
            sum=0;

            for(hr_data=0;hr_data<56;hr_data++){
                sum=sum+readings[first_data+hr_data];
                }

            m[day][14]=sum/56;
    }

    //SD
    for(day=0;day<30;day++){
        first_data=((56*day)+a);
        sum=0;

        for(hr_data=0;hr_data<56;hr_data++){
            sum=sum+pow((readings[first_data+hr_data]-m[day][14]),2);
        }
        m[day][15]=sqrt(sum/56);

    }

}

float monthhr(float month[30][16],int N_hr){

    float sum=0,result;
    int day;

    for(day=0;day<30;day++){
        sum=sum+(month[day][N_hr]);
    }
    result=sum/30;
    return result;

}

void table(){

    int i;
    float b;

    printf("\n\n");
    printf("\n");
    for(i=1;i<11;i++){
        printf("~~~~~");
    }
    printf(" Time (N Hour) ");
    for(i=1;i<11;i++){
        printf("~~~~~");
    }
    printf("\n");
    for(b=7;b<13;b++){
        printf("%.2f AM|",b);

    }
    for(b=1;b<9;b++){
        printf("%.2f PM|",b);
    }
    printf("\n");
    for(i=1;i<24;i++){
        printf("-----");
    }
    printf("\n");
    for(i=0;i<3;i++){
        printf("   %d   |",i);
    }
    for(i=3;i<6;i++){
        printf("    %d   |",i);
    }
    for(i=6;i<10;i++){
        printf("   %d   |",i);
    }
    for(i=10;i<14;i++){
        printf("  %d   |",i);
    }
    printf("\n\n");
    printf("~~ Calender (N Month) ~~");
    printf("\n");
    printf("%6s |%5s  |%6s |\n","April","May","June");
    for(i=1;i<5;i++){
        printf("------");
    }
    printf("\n");
    for(i=0;i<3;i++){
        printf("   %d   |",i);
    }

}

void prt(float month[30][16]){

    int row,col,x;
    float b;

    printf("\n");
    for(x=1;x<14;x++){
        printf("~~~");
    }
    printf(" Average by Hour ");

    for(x=1;x<13;x++){
        printf("~~~");
    }

    printf("\n");
    printf("          ");

    for(b=7;b<10;b++){
        printf(" %.0fAM |",b);

    }
    for(b=10;b<13;b++){
        printf("%.0fPM |",b);

    }
    for(b=1;b<9;b++){
        printf(" %.0fPM |",b);

    }
    printf("Mean |");
    printf(" SD |");
    printf("\n");
    for(x=1;x<22;x++){
        printf("-----");
    }


    for (row=0;row<30;row++){
            printf("\nDay %-2d:  |", row+1);
            for(col=0;col<16;col++){
                printf("%.2f|",month[row][col]);
            }
    }
}

void getInput(int *N_hr, int *N_mth) {
    printf("\n\nCalculate monthly average of? (Enter -1 to end program)\n");
    printf("\nEnter N Hour : ");
    scanf("%d", N_hr);  // Read input into the address pointed by N_hr
    printf("Enter N Month  : ");
    scanf("%d", N_mth);  // Read input into the address pointed by N_mth
}

int main() {
    FILE *file = fopen("proj2.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    float readings[max];
    float april[30][16];
    float may[30][16];
    float june[30][16];

    int col=0,N_hr,N_mth;
    float result;

    while (fscanf(file, "%f", &readings[col]) != EOF && col < max) {
        col++;
    }

    fclose(file);

    hourave(readings,april,0);
    hourave(readings,may,1680);
    hourave(readings,june,3360);

    dailymsd(readings,april,0);
    dailymsd(readings,may,1680);
    dailymsd(readings,june,3360);

    printf("APRIL :");
    prt(april);
    printf("\n\nMAY :");
    prt(may);
    printf("\n\nJUNE :");
    prt(june);

    table();
    getInput(&N_hr, &N_mth);

    while(((N_hr<0)||(N_hr>13))||((N_mth<0)||(N_mth>2))){
        if(N_hr==-1){
            return 0;
        }
        if(N_mth==-1){
            return 0;
        }
        printf("\nInvalid input N\n");
        getInput(&N_hr, &N_mth);

    }


    if(N_mth==0){
        result=monthhr(april,N_hr);
    }
    else if(N_mth==1){
        result=monthhr(may,N_hr);
    }
    else if(N_mth==2){
        result=monthhr(june,N_hr);
    }


    printf("\nMonthly average : %.2f\n",result);


    return 0;
}
//#endif
