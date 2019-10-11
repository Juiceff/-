void * Select(void *arg);     //�ѿ��������� 
void sum(select_result * re1,int num);  //��ͺ��� 
void * sort();                     //������ 
int change_argv_to_number(char s[]);   //���ַ���ת��Ϊ���� 

select_result *re2;
int count=0;         //���� 
static sem_t sem_one;                      //�����첽�ź��� 

void * Select(void *arg)//���еѿ����� 
{
	
	argg *arg1;
	arg1=(argg *)arg;
	customer *cus=(customer *)arg1->cus;
	orders *ord=(orders *)arg1->ord;
	lineitem *item=(lineitem *)arg1->item;
	char *order_date=(char *)arg1->order_date;
	char *ship_date=(char *)arg1->ship_date;
	char *mktsegment=(char *)arg1->mktsegment;
	int begin=((int *)arg1->range)[0];
	int end=((int *)arg1->range)[1];    //���̴߳��͵�void *���͵Ĳ�������ǿ������ת�� 
	int i,j,k,l=0;
	select_result * result1=NULL;
	result1 = (select_result *)malloc(340*sizeof(select_result));
	//result2 = (select_result *)malloc(1001*sizeof(select_result));
	//printf("%d\n",end);
	for(j=begin;j<end;j++)            //�ѿ���������ʼ����ֹ�� 
	{
		for(i=0;i<100;i++)
		{
			for(k=0;k<1000;k++)
			if(cus[i].c_custkey==ord[j].o_custkey&&ord[j].o_orderkey==item[k].l_orderkey&&(strcmp(ord[j].o_orderdate,order_date)<0)&&(strcmp(item[k].l_shipdate,ship_date)>0)&&(strcmp(cus[i].c_mkgsegment,mktsegment)==0))
			{
				//printf("%d,%s,%lf\n",item[k].l_orderkey,ord[j].o_orderdate,item[k].l_extendedprice);
				result1[l].l_orderkey=item[k].l_orderkey;
				strcpy(result1[l].o_orderdate,ord[j].o_orderdate);
				result1[l].l_extendedprice=item[k].l_extendedprice;
				l++;
				
			}
		}
	}
	sem_wait(&sem_one);              
	sum(result1,l);
	sem_post(&sem_one);                        //�Եѿ�������������Ͳ��������첽���� 
	free(result1);
	//printf("%d,%s,%lf\n",re2[0].l_orderkey,re2[0].o_orderdate,re2[0].l_extendedprice);
	return NULL;//����ѡ������ָ�� 
}

void sum(select_result * re1,int num)
{
	
	int i=0,j=0;
	//printf("%d",num);
		for(i=0;i<num;i++)
		{

			if(re2[0].l_orderkey==0)
			{
			re2[j].l_orderkey = re1[i].l_orderkey;
			strcpy(re2[j].o_orderdate,re1[i].o_orderdate);
			re2[j].l_extendedprice = re1[i].l_extendedprice;
			
			continue;
			}
			if(re1[i].l_orderkey==re2[j].l_orderkey)
			{
				re2[j].l_extendedprice = re2[j].l_extendedprice + re1[i].l_extendedprice;
			//printf("%d,%s,%lf\n",re2[j].l_orderkey,re2[j].o_orderdate,re2[j].l_extendedprice);
			}
			else
			{
			
				j++;count++;
				re2[j].l_orderkey = re1[i].l_orderkey;
				strcpy(re2[j].o_orderdate,re1[i].o_orderdate);
				re2[j].l_extendedprice = re1[i].l_extendedprice;
			}
	
		}
}


void * sort()//����ѡ�� 
{
	int i,j;
	select_result  temp;
	for(i=0;i<count-1;i++)//ð������ ����ѡ������Ϊ���� 
	{
		for(j=0;j<count-1-i;j++)
		{
			//printf("%lf->%lf\n",result2[j].l_extendedprice,result2[j+1].l_extendedprice);
			if(re2[j].l_extendedprice<re2[j+1].l_extendedprice)
			{
				
				temp.l_extendedprice=re2[j].l_extendedprice;
				temp.l_orderkey=re2[j].l_orderkey;
				strcpy(temp.o_orderdate,re2[j].o_orderdate);
				re2[j].l_extendedprice=re2[j+1].l_extendedprice;
				re2[j].l_orderkey=re2[j+1].l_orderkey;
				strcpy(re2[j].o_orderdate,re2[j+1].o_orderdate);
				re2[j+1].l_extendedprice=temp.l_extendedprice;
				re2[j+1].l_orderkey=temp.l_orderkey;
				strcpy(re2[j+1].o_orderdate,temp.o_orderdate);
			}
		}
	}
}

int change_argv_to_number(char s[])//�������������������ַ���ת��Ϊ�������� 
{
	int i=0;
	int number=0;
	while(s[i]!='\0')
	{
		if(i==0)
			number = (s[i]-48);
		else
			number = number*10 + (s[i]-48);
		//printf("%d,%d\n",i,number);
		i++;
	}
	return number;
}
