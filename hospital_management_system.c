#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

struct patient{
    int id;
    char patientName[50];
    int age;
    char gender;
    char patientAddress[50];
    char disease[50];
    char date[12];
    int consultationDoctorId;
    int covidStatus;
}p;

struct doctor{
    int id;
    char name[50];
    char address[50];
    char specialize[50];
    char date[12];
    int consultationFee;
    int isCovidConsultantDoctor;
}d;

struct covidDetails{
    int patientId;
    int id;
    float bodyTemperature;
    char lossOfSmellOrTaste;
    char hasDryCough;
    char hasShortnesssOfBreath;
    char hasFatigue;
}c;

FILE *fp;

void admitPatient();
void patientList();
void dischargePatient();
void editPatientDetails();
void addDoctor();
void doctorList();
void searchPatient();
void searchDoctor();
void deleteDoctor();
int checkIfDuplicate(int,char*);
void editDoctorDetails();
void generateBill(int);
void getCovidDetailsById();
int checkForCovid(int);
int deleteCovidDetailsById(int);

int main(){

    int ch;

    while(1){
        system("cls");
        printf("<== Hospital Management System ==>\n");
        printf("1.Admit Patient\n");//done
        printf("2.Patient List\n");//done
        printf("3.Discharge Patient\n"); //delete related covid details, covid test amount in bill
        printf("4.Add Doctor\n"); //done
        printf("5.Doctors List\n"); //printing update
        printf("6.Search Patient\n"); //printing update
        printf("7.Search Doctor\n"); //printing update
        printf("8.Delete Doctor\n"); //printing update
        printf("9.Edit Patient Details\n"); //add calling edit covid details
        printf("10.Edit Doctor Details\n"); // add covid status
        printf("11.Search Covid Details\n");//done
        printf("0.Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch(ch){
        case 0:
            exit(0);

        case 1:
            admitPatient();
            break;

        case 2:
            patientList();
            break;

        case 3:
            dischargePatient();
            break;

        case 4:
            addDoctor();
            break;

        case 5:
            doctorList();
            break;

        case 6:
            searchPatient();
            break;
        case 7:
            searchDoctor();
            break;
        case 8:
            deleteDoctor();
            break;
        case 9:
            editPatientDetails();
            break;
        case 10:
            editDoctorDetails();
            break;
        case 11:
            getCovidDetailsById();
            break;
        default:
            printf("Invalid Choice...\n\n");

        }
        printf("\n\nPress Any Key To Continue...");
        getch();
    }

    return 0;
}

int checkIfDuplicate(int id, char *fileName){
    FILE *fp;

    fp = fopen(fileName, "rb");
    while(fread(&p, sizeof(p), 1, fp) == 1){

        if(id == p.id){
            return 1;
        }
    }
    return 0;
}

int checkForCovid(int patientId){
    int temp,f=0;

    FILE* ft = fopen("covidDetails.txt", "ab");

    do{
        printf("Enter covid id for the patient: ");
        scanf("%d", &temp);
        if(!checkIfDuplicate(temp,"covidDetails.txt")){
            c.id= temp;
            f=0;
        }
        else {
            f=1;
            printf("\nCovid id already exists, assign a new one!!!\n\n");
        }
    }while(f==1);

    c.patientId = patientId;
    
    printf("Enter Patient body temperature in Farenheit: ");
    fflush(stdin);
    scanf("%f",&c.bodyTemperature);

    printf("Does patient has loss of taste/smell status(Y/N): ");
    fflush(stdin);
    scanf("%c",&c.lossOfSmellOrTaste);

    printf("Does patient has sore throat or cough(Y/N): ");
    fflush(stdin);
    scanf("%c",&c.hasDryCough);

    printf("Does patient has shortness of breath or difficulty breathing(Y/N): ");
    fflush(stdin);
    scanf("%c",&c.hasShortnesssOfBreath);

    printf("Does patient has tiredness or fatigue or muscle aches(Y/N): ");
    fflush(stdin);
    scanf("%c",&c.hasFatigue); 

    fwrite(&c, sizeof(c), 1, ft);
    fclose(ft);

    int totalSymptomsPresent= 0; 
    totalSymptomsPresent += c.bodyTemperature> 104;
    totalSymptomsPresent += (c.lossOfSmellOrTaste == 'y' || c.lossOfSmellOrTaste == 'Y');
    totalSymptomsPresent += (c.hasDryCough == 'y' || c.hasDryCough == 'Y');
    totalSymptomsPresent += (c.hasFatigue == 'y' || c.hasFatigue == 'Y');
    totalSymptomsPresent += (c.hasShortnesssOfBreath == 'y' || c.hasShortnesssOfBreath == 'Y');

    if(totalSymptomsPresent >= 3)
        return 1;
    else 
        return 0;
}

void admitPatient(){
    int temp,f=0;
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(p.date, myDate);

    fp = fopen("patient.txt", "ab");

    do{
        printf("Enter Patient id: ");
        scanf("%d", &temp);
        if(!checkIfDuplicate(temp,"patient.txt")){
            p.id= temp;
            f=0;
        }
        else {
            f=1;
            printf("\nPatient with this id already exists!!!\n\n");
        }
    }while(f==1);

    printf("Enter Patient name: ");
    fflush(stdin);
    scanf("%s",p.patientName);

    printf("Enter Patient age: ");
    fflush(stdin);
    scanf("%d",&p.age);

    printf("Enter Patient gender(F/M): ");
    fflush(stdin);
    scanf("%c",&p.gender);

    printf("Enter Patient Address: ");
    fflush(stdin);
    scanf("%s",p.patientAddress);

    printf("Enter Patient Disease: ");
    fflush(stdin);
    scanf("%s",p.disease);

    printf("---------Checking symptoms for COVID19----------------\n");

    p.covidStatus = checkForCovid(p.id);

    printf("<== Doctor List ==>\n\n");
    printf("%-10s %-20s %-20s %-20s %-20s %s\n", "id", "Name", "Address", "Specialize", "Consulation Fee", "Date");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    FILE* ft = fopen("doctor.txt", "rb");
    if(p.covidStatus == 1){
        while(fread(&d, sizeof(d), 1, ft) == 1){
            if(d.isCovidConsultantDoctor == 1)
                printf("%-10d %-20s %-20s %-20s %-20d %s\n", d.id, d.name, d.address, d.specialize, d.consultationFee, d.date);
        }
    }
    else{
        while(fread(&d, sizeof(d), 1, ft) == 1){
            printf("%-10d %-20s %-20s %-20s %-20d %s\n", d.id, d.name, d.address, d.specialize, d.consultationFee, d.date);
        }
    }

    fclose(ft);

    printf("\nChoose doctor from the list\n\n");
    printf("Enter concerned doctor id: ");
    fflush(stdin);
    scanf("%d",&p.consultationDoctorId);

    fwrite(&p, sizeof(p), 1, fp);
    printf("\nPatient Added Successfully");

    
    fclose(fp);
}

void patientList(){

    char* temp;
    system("cls");
    printf("<== Patient List ==>\n\n");
    printf("%-10s %-20s %-10s %-10s %-20s %-10s %-25s %s\n", "Id", "Patient Name", "Age", "Gender", "Address", "Disease", "Covid symptoms status", "Date");
    printf("-------------------------------------------------------------------------------------------------------------------------------------\n");

    fp = fopen("patient.txt", "rb");
    while(fread(&p, sizeof(p), 1, fp) == 1){
        if(p.covidStatus == 1)
            temp="yes";
        else
            temp="no";
        printf("%-10d %-20s %-10d %-10c %-20s %-10s %-25s %s\n", p.id, p.patientName, p.age, p.gender, p.patientAddress, p.disease, temp, p.date);
    }

    fclose(fp);
}


void dischargePatient(){
    int id, f=0;
    system("cls");
    printf("<== Discharge Patient ==>\n\n");
    printf("Enter Patient id to discharge: ");
    scanf("%d", &id);

    FILE *ft;

    fp = fopen("patient.txt", "rb");
    ft = fopen("temp.txt", "wb");

    while(fread(&p, sizeof(p), 1, fp) == 1){

        if(id == p.id){
            f=1;
            generateBill(id);
            deleteCovidDetailsById(id);
        }else{
            fwrite(&p, sizeof(p), 1, ft);
        }
    }

    if(f==1){
        printf("\n\nPatient Discharged Successfully.");
    }else{
        printf("\n\nRecord Not Found !");
    }

    fclose(fp);
    fclose(ft);

    remove("patient.txt");
    rename("temp.txt", "patient.txt");
    
}

void searchPatient(){
    int id,f=0;
    char* temp;
    system("cls");
    printf("<== Search Patient ==>\n\n");
    printf("Enter Patient id to search: ");
    scanf("%d", &id);

    FILE *fp;

    fp = fopen("patient.txt", "rb");
    while(fread(&p, sizeof(p), 1, fp) == 1){

        if(id == p.id){
            f=1;
            printf("%-10s %-20s %-10s %-10s %-20s %-10s %-25s %s\n", "Id", "Patient Name", "Age", "Gender", "Address", "Disease", "Covid symptoms status", "Date");
            printf("-------------------------------------------------------------------------------------------------------------------\n");
            if(p.covidStatus == 1)
                temp="yes";
            else
                temp="no";
            printf("%-10d %-20s %-10d %-10c %-20s %-10s %-25s %s\n", p.id, p.patientName, p.age, p.gender, p.patientAddress, p.disease, temp, p.date);        }
    }

    if(f==0){
        printf("\n\nRecord Not Found !");
    }

    fclose(fp);
}

void editPatientDetails(){
    int id,f=0;
    char* temp;
    system("cls");
    printf("<== Update Patient ==>\n\n");
    printf("Enter Patient id to search: ");
    scanf("%d", &id);

    FILE *ft;

    fp = fopen("patient.txt", "rb");
    ft = fopen("temp.txt", "wb");
    while(fread(&p, sizeof(p), 1, fp) == 1){

        if(id == p.id){
            f=1;
            printf("\n-----------------------------------------Patient Details------------------------------------------------\n");
            printf("%-10s %-30s %-10s %-15s %-20s %-10s %-20s %s\n", "Id", "Patient Name", "Age", "Gender", "Address", "Disease", "Covid symptoms status", "Date");
            printf("------------------------------------------------------------------------------------------------------------------\n");
            if(p.covidStatus == 1)
                temp="yes";
            else
                temp="no";
            printf("%-10d %-30s %-10d %-15c %-20s %-10s %-20s %s\n", p.id, p.patientName, p.age, p.gender, p.patientAddress, p.disease, temp, p.date);            
            printf("\n----------------------------------------Enter new details-----------------------------------------------\n");
            printf("Enter Patient name: ");
            fflush(stdin);
            scanf("%s",p.patientName);

            printf("Enter Patient age: ");
            fflush(stdin);
            scanf("%d",&p.age);

            printf("Enter Patient gender: ");
            fflush(stdin);
            scanf("%c",&p.gender);

            printf("Enter Patient Address: ");
            fflush(stdin);
            scanf("%s",p.patientAddress);

            printf("Enter Patient Disease: ");
            fflush(stdin);
            scanf("%s",p.disease);

            printf("<== Doctor List ==>\n\n");
            printf("%-10s %-20s %-20s %-20s %-20s %s\n", "id", "Name", "Address", "Specialize", "Consulation Fee", "Date");
            printf("-------------------------------------------------------------------------------------------------------------------\n");

            FILE* fd = fopen("doctor.txt", "rb");
            if(p.covidStatus == 1){
                while(fread(&d, sizeof(d), 1, fd) == 1){
                    if(d.isCovidConsultantDoctor == 1)
                        printf("%-10d %-20s %-20s %-20s %-20d %s\n", d.id, d.name, d.address, d.specialize, d.consultationFee, d.date);
                }
            }
            else{
                while(fread(&d, sizeof(d), 1, ft) == 1){
                    printf("%-10d %-20s %-20s %-20s %-20d %s\n", d.id, d.name, d.address, d.specialize, d.consultationFee, d.date);
                }
            }

            fclose(fd);

            printf("\nChoose doctor from the list\n\n");
            printf("Enter concerned doctor id: ");
            fflush(stdin);
            scanf("%d",&p.consultationDoctorId);

            fwrite(&p, sizeof(p), 1, ft);
        }
        else{
            fwrite(&p, sizeof(p), 1, ft);
        }
    }

    if(f==1){
        printf("\n\nPatient Details Updated Successfully!!!\n");
    }else{
        printf("\n\nRecord Not Found !\n");
    }

    fclose(fp);
    fclose(ft);

    remove("patient.txt");
    rename("temp.txt", "patient.txt");
}

void addDoctor(){

    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(d.date, myDate);

    int f=0,temp;

    system("cls");
    printf("<== Add Doctor ==>\n\n");

    FILE* fp = fopen("doctor.txt", "ab");

    do{
        printf("Enter Doctor id: ");
        scanf("%d", &temp);
        if(!checkIfDuplicate(temp,"doctor.txt")){
            d.id= temp;
            f=0;
        }
        else {
            f=1;
            printf("\nDoctor with this id already exists!!!\n\n");
        }
    }while(f==1);

    printf("Enter Doctor Name: ");
    fflush(stdin);
    scanf("%s",d.name);

    printf("Enter Doctor Address: ");
    fflush(stdin);
    scanf("%s",d.address);

    printf("Doctor Specialize in: ");
    fflush(stdin);
    scanf("%s",d.specialize);

    printf("Doctor consultation fee: ");
    fflush(stdin);
    scanf("%d",&d.consultationFee);

    printf("Is this doctor assigned to take covid cases(Y/N): ");
    fflush(stdin);
    char tempChar;
    scanf("%c",&tempChar);
    if(temp == 'Y' || temp =='y')
        d.isCovidConsultantDoctor= 1;
    else
        d.isCovidConsultantDoctor=0;

    fwrite(&d, sizeof(d), 1, fp);
    printf("Doctor Added Successfully\n\n");

    fclose(fp);
}

void doctorList(){
    system("cls");
    char* temp;
    printf("<== Doctor List ==>\n\n");

    printf("%-10s %-20s %-20s %-20s %-20s %-20s %s\n", "id", "Name", "Address", "Specialize", "Consulation Fee", "Covid Consultancy status", "Date");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    fp = fopen("doctor.txt", "rb");
    while(fread(&d, sizeof(d), 1, fp) == 1){
        if(d.isCovidConsultantDoctor == 1)
            temp="yes";
        else
            temp="no";
        printf("%-10d %-20s %-20s %-20s %-20d %-20s %s\n", d.id, d.name, d.address, d.specialize, d.consultationFee, temp, d.date);
    }

    fclose(fp);
}

void searchDoctor(){
    int id,f=0;
    char* temp;
    system("cls");
    printf("<== Search Doctor ==>\n\n");
    printf("Enter Doctor id to search: ");
    scanf("%d", &id);

    FILE *fp;

    fp = fopen("doctor.txt", "rb");
    while(fread(&d, sizeof(d), 1, fp) == 1){

        if(id == d.id){
            f=1;
            printf("%-10s %-20s %-20s %-20s %-20s %-20s %s\n", "id", "Name", "Address", "Specialize", "Consulation Fee", "Covid Consultancy status", "Date");
            printf("-------------------------------------------------------------------------------------------------------------------\n");
            if(d.isCovidConsultantDoctor == 1)
                temp="yes";
            else
                temp="no";
            printf("%-10d %-20s %-20s %-20s %-20d %-20s %s\n", d.id, d.name, d.address, d.specialize, d.consultationFee, temp, d.date);        }
    }

    if(f==0){
        printf("\n\nRecord Not Found !");
    }

    fclose(fp);
}

void deleteDoctor(){
    int id, f=0;
    system("cls");
    printf("<== Delete Doctor Record ==>\n\n");
    printf("Enter Doctor id to remove: ");
    scanf("%d", &id);

    FILE *ft;

    fp = fopen("doctor.txt", "rb");
    ft = fopen("temp.txt", "wb");

    while(fread(&d, sizeof(d), 1, fp) == 1){

        if(id == d.id){
            f=1;
        }else{
            fwrite(&d, sizeof(d), 1, ft);
        }
    }

    if(f==1){
        printf("\n\nDoctor Record Removed Successfully.");
    }else{
        printf("\n\nRecord Not Found !");
    }

    fclose(fp);
    fclose(ft);

    remove("doctor.txt");
    rename("temp.txt", "doctor.txt");

}

void editDoctorDetails(){
    int id,f=0;
    char* temp;
    system("cls");
    printf("<== Search Doctor ==>\n\n");
    printf("Enter Doctor id to search: ");
    scanf("%d", &id);

    FILE *ft;

    fp = fopen("doctor.txt", "rb");
    ft = fopen("temp.txt", "wb");
    while(fread(&d, sizeof(d), 1, fp) == 1){

        if(id == d.id){
            f=1;
            printf("\n-----------------------------------------Doctor Details------------------------------------------------\n");
            printf("%-10s %-20s %-20s %-20s %-20s %-20s %s\n", "id", "Name", "Address", "Specialize", "Consulation Fee", "Covid Consultancy status", "Date");
            printf("---------------------------------------------------------------------------------------------------------\n\n");
            if(d.isCovidConsultantDoctor == 1)
                temp="yes";
            else
                temp="no";
            printf("%-10d %-20s %-20s %-20s %-20d %-20s %s\n", d.id, d.name, d.address, d.specialize, d.consultationFee, temp, d.date);            
            
            printf("\n----------------------------------------Enter new details----------------------------------------------\n");
            printf("Enter Doctor Name: ");
            fflush(stdin);
            scanf("%s",d.name);

            printf("Enter Doctor Address: ");
            fflush(stdin);
            scanf("%s",d.address);

            printf("Doctor Specialize in: ");
            fflush(stdin);
            scanf("%s",d.specialize);

            printf("Doctor consultation fee: ");
            fflush(stdin);
            scanf("%d",&d.consultationFee);

            printf("Is this doctor assigned to take covid cases(Y/N): ");
            fflush(stdin);
            char tempChar;
            scanf("%c",&tempChar);
            if(temp == 'Y' || temp =='y')
                d.isCovidConsultantDoctor= 1;
            else
                d.isCovidConsultantDoctor=0;

            fwrite(&d, sizeof(d), 1, ft);
        }
        else{
            fwrite(&d, sizeof(d), 1, ft);
        }
    }

    if(f==1){
        printf("\n\nDoctor Details Updated Successfully!!!\n");
    }else{
        printf("\n\nRecord Not Found !\n");
    }

    fclose(fp);
    fclose(ft);

    remove("doctor.txt");
    rename("temp.txt", "doctor.txt");
}

void generateBill(int patientId){

    int taxPercentage= 10, f=0, doctorId;
    double tax=0, totalAmount =0, covidTestCharges = 1500;

    FILE *fp;

    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(d.date, myDate);

    fp = fopen("patient.txt", "rb");
    while(fread(&p, sizeof(p), 1, fp) == 1){

        if(patientId == p.id){
            f=1;
            FILE *ft;
            doctorId= p.consultationDoctorId;
            ft = fopen("doctor.txt", "rb");
            while(fread(&d, sizeof(d), 1, ft) == 1){

                if(doctorId == d.id){
                    printf("----------------------------------------------------------------------------------------------------------\n");
                    printf("                                                   BILL                                                   \n");
                    printf("----------------------------------------------------------------------------------------------------------\n");
                    printf("%-15s: %s %60s: %s\n", "Patient Name", p.patientName,"Date",myDate);
                    printf("%-5s: %d %80s: %s\n","Age", p.age, "Doctor Name",d.name);
                    printf("%-8s: %c\n","Gender", p.gender);
                    printf("----------------------------------------------------------------------------------------------------------\n");
                    printf("%-50s Rs.%50lf\n","Consultation fee", (float)d.consultationFee);
                    if(p.covidStatus == 1) printf("%-50s Rs.%50lf\n", "Covid test charges", covidTestCharges);
                    tax= (taxPercentage/100.0) * d.consultationFee;
                    printf("%-50s Rs.%50lf\n","GST taxes",tax);
                    totalAmount = tax + d.consultationFee;
                    totalAmount += p.covidStatus == 1 ?  covidTestCharges : 0;
                    printf("----------------------------------------------------------------------------------------------------------\n");
                    printf("%-50s Rs.%50lf\n", "Total Amount", totalAmount);
                    printf("----------------------------------------------------------------------------------------------------------\n");
                }
            }
            fclose(ft);
        }
    }

    if(f==0){
        printf("\n\nRecord Not Found !");
    }

    fclose(fp);

}

void getCovidDetailsById(){
    char* temp,f=0;
    int id;
    FILE* fp;
    system("cls");
    printf("Enter pateint id to get covid details:\n");
    scanf("%d",&id);

    printf("<== Patient List ==>\n\n");
    printf("%-10s %-30s %-10s %-15s %-20s %-10s %-30s %s\n", "Id", "Patient Name", "Age", "Gender", "Address", "Disease", "Covid symptoms status", "Date");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------\n");

    fp = fopen("patient.txt", "rb");
    while(fread(&p, sizeof(p), 1, fp) == 1){
        if(p.id == id){
            if(p.covidStatus == 1)
                temp="yes";
            else
                temp="no";
            printf("%-10d %-30s %-10d %-15c %-20s %-10s %-30s %s\n\n", p.id, p.patientName, p.age, p.gender, p.patientAddress, p.disease, temp, p.date);
        }
    }

    FILE* ft = fopen("covidDetails.txt", "rb");
    while(fread(&c, sizeof(c), 1, ft) == 1){
        if(c.patientId == id){
            printf("\n----------------------------------------Deatiled report on patient-----------------------------------------------------");
            printf("This patient-%d has a body temperature of %f and other symptoms such as", c.id, c.bodyTemperature);
            if(c.hasDryCough == 'y'|| c.hasDryCough == 'Y') printf("-- sore throat or dry cough\n");
            if(c.hasFatigue == 'y' || c.hasFatigue == 'Y') printf("-- fatigue or tiredness with muscle aches\n");
            if(c.hasShortnesssOfBreath == 'y' || c.hasShortnesssOfBreath == 'Y') printf("-- has shortness of breath or difficulty breathing\n");
            if(c.lossOfSmellOrTaste == 'y' || c.lossOfSmellOrTaste == 'y') printf("-- has loss of loss of taste and smell\n");
        }
    }

    fclose(fp); 
    fclose(ft);
}

int deleteCovidDetailsById(int patientId){
    FILE* ft = fopen("covidDetails.txt", "rb");
    while(fread(&c, sizeof(c), 1, ft) == 1){
        if(c.patientId != patientId){
            fwrite(&c, sizeof(c), 1, ft);
        }
    }
    fclose(ft);

    remove("covidDetails.txt");
    rename("temp.txt", "covidDetails.txt");
}
    