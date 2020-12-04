#include <cstdio>
#include <cstdlib>
#include <cmath>

struct List{
    double elm;
    struct List *next;
};

int get_array(List *data){
    int i = 1;
    List *q = data;
    scanf("%lf", &data->elm);
    do{
        List *p = (List*)malloc(sizeof(List));
        scanf("%lf", &p->elm);
        q->next = p;
        p->next = nullptr;
        q = p;
        i++;
    }while(getchar() != '\n');
    return i;
}

void print_array(List *data){
    List *p = data;
    while(p!=nullptr){
        printf("%lf  ", p->elm);
        p = p->next;
    }
    printf("\n");
}

double get_average(List *data, int num){
    double sum = 0.0;
    List *p = data;
    while(p!= nullptr){
        sum += p->elm;
        p = p->next;
    }
    return sum / num;
}

double get_A_uncertainty(List *data, int *num){
    double ave = get_average(data, *num);
    double sum = 0.0;
    List *p = data;
    while(p!= nullptr){
        sum += pow(p->elm - ave, 2);
        p = p->next;
    }
    sum = pow(sum / (*num-1), 0.5);

    int flag;
    do{
        double judge = sum * 3;
        p = data;
        List *q;
        flag = 0;
        while(p != nullptr){
            if((std::abs(p->elm - ave)) >= judge){
                flag = 1;
                printf("\nOne bad value: %lf \n\n", p->elm);
                (*num)--;
                if(p == data)
                    data = p->next;
                else {
                    q->next = p->next;
                }
            }
            q = p;
            p = p->next;
        }

        if(flag){
            p = data;
            ave = get_average(data, *num);
            sum = 0.0;

            while(p!= nullptr){
                sum += pow(p->elm - ave, 2);
                p = p->next;
            }
            sum = pow(sum / (*num-1), 0.5);
        }
    }while(flag);

    sum = sum / pow(*num, 0.5);

    switch(*num){
        case 2:
            sum *= 1.84;
            break;
        case 3:
            sum *= 1.32;
            break;
        case 4:
            sum *= 1.20;
            break;
        case 5:
            sum *= 1.14;
            break;
        default:
            break;
    }

    return sum;
}

double get_B_uncertainty(double equ){
    double three_root = pow(3, 0.5);
    double val = equ / three_root;
    return val;
}

double get_uncertainty(double a, double b){
    double val = a*a + b*b;
    val = pow(val, 0.5);
    return val;
}

int main(){
    List *data = (List*)malloc(sizeof(List));
    data->next = nullptr;

    puts("Please input the data split with spaces:");
    int num = get_array(data);

    puts("PLease input the uncertainty of the equipment:");
    double delta_equ;
    scanf("%lf", &delta_equ);
    double a = get_A_uncertainty(data, &num);
    double b = get_B_uncertainty(delta_equ);
    double uncertainty = get_uncertainty(a,b);

    printf("\na: %lf\nb: %lf\nfinal: %lf\n", a, b, uncertainty);

    puts("### Remember to take the effective number yourself. ###");
    system("pause");
    return 0;
}
