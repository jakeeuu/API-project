#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CARATTERI 64
#define MAXLEN 17
//la lunghezza massima dei comandi è di 17

typedef struct controll_vect{
	int8_t exist;// -1 = boh, 0=not exits , 1 = exist(>=), 2= exist and exactly
	uint8_t count;
}conv;

typedef struct double_vector{
	char let;
	uint8_t pos;
}dvect;

typedef struct diz_tree{
	struct diz_tree * p;
	struct diz_tree * dx;
	struct diz_tree * sx;
	char color;
	char word[];
}tree;

typedef struct elem_s{
	struct elem_s * next;
	char word[];
}elem_t;

void inizial_lett(conv []);
void inizial_PP(uint64_t [],char [],uint8_t);

tree * primi_elementi(tree * , char [],uint8_t,int *);
elem_t * nuovi_list(elem_t * , char [],int *,uint8_t , conv [],uint64_t [],char [], uint8_t []);
tree * nuovi_tree(tree * t, char [],uint8_t );

void creo_ist(char [],uint8_t []);

tree * inserisci(tree * ,tree * );
int ord_less(char [],char []);
tree * riparaRBinserisci(tree * ,tree * );
tree * left_rotate(tree * ,tree * );
tree * right_rotate(tree * ,tree * );

char rev_vettpos(int8_t );
int8_t vettpos(char );

int exist(char [],tree * );
void confronti(char [],char [],uint8_t [],char [],dvect [],char []);

void delete(tree * );

void cambiamenti(uint64_t [],char [],conv [],dvect [],char [],int8_t [],int8_t []);
elem_t * camb_tree(tree * ,int *,conv [],char [],char [],int8_t [],int8_t [],uint8_t [],elem_t *,uint8_t);

elem_t * canc(elem_t * );
elem_t * pop(elem_t * );
void print(elem_t * );
elem_t * push(elem_t * ,char [],uint8_t );
elem_t * mergesort(elem_t * ,int );
elem_t * merge(elem_t *, elem_t *);

elem_t * camb_list(elem_t * ,int * ,conv [],char [],char [],int8_t [],int8_t [],uint8_t []);

void stampa_tree(tree * );

int main(int argc, char*argv[]){
	uint8_t k;
	uint8_t j=MAXLEN;
	int8_t attempt;
	int count;
	int elem=0;

	elem_t * head=NULL;
	tree * root=NULL;
	conv lettere[CARATTERI];

	if(scanf("%hhd",&k));
	if(k>MAXLEN){
		j=k;
	}
	
	char parola[j+1];
	
	char rif[k+1];
	char output[k+1];
	uint8_t istogramma[CARATTERI];
	char rif2[k+1];
	dvect parola2[k+1];

	int8_t pos_nuove[k+1];
	int8_t ist_nuove[k+1];

	char plus[k+1];
	uint64_t pipe[k];

	root=primi_elementi(root,parola,k,&elem);

	while(!feof(stdin)){
		if(parola[0]=='+' && parola[1]=='n'){
			inizial_lett(lettere);//inizializzo lettere
			inizial_PP(pipe,plus,k);//inizializzo pipe e plus
			count=elem;//inizializzo count
			if(scanf("%s",rif));
			if(scanf("%hhd",&attempt));
			while(attempt!=0){
				if(scanf("%s",parola));
				if(parola[0]=='+' && parola[1]=='i'){//+inserisci_inizio
					if(head!=NULL){
						if(scanf("%s",parola));
						while(parola[0]!='+'){//il + è quello di +inserisci_fine
							elem++;
							root=nuovi_tree(root,parola,k);
							head=nuovi_list(head,parola,&count,k,lettere,pipe,plus,istogramma);
							if(scanf("%s",parola));
						}
					}else{
						root=primi_elementi(root,parola,k,&elem);
						count=elem;
					}
					
				}else if(parola[0]=='+' && parola[1]=='s'){//+stampa_filtrate
					if(head!=NULL){
						head=mergesort(head,count);
						print(head);
					}else{
						stampa_tree(root);
					}
				}else{
					if(exist(parola,root)){
						attempt--;
						if(ord_less(parola,rif)==2){
							printf("ok\n");
							attempt=0;
						}else{
							confronti(rif,rif2,istogramma,parola,parola2,output);
							printf("%s\n",output);
							cambiamenti(pipe,plus,lettere,parola2,output,pos_nuove,ist_nuove);
							if(head!=NULL){
								head=camb_list(head,&count,lettere,parola,output,pos_nuove,ist_nuove,istogramma);
							}else{
								head=camb_tree(root,&count,lettere,parola,output,pos_nuove,ist_nuove,istogramma,head,k);
							}
							printf("%d\n",count);
							if(attempt==0)
								printf("ko\n");
						}
					}else{
						printf("not_exists\n");
					}
				}
			}

		//cancello elementi dalla lista
		while(head!=NULL)
			head=pop(head);

		}else if(parola[0]=='+' && parola[1]=='i'){
			root=primi_elementi(root,parola,k,&elem);
		}
		if(scanf("%s",parola));
	}

	//cancello albero rb
	delete(root);
	return 0;
}

void inizial_lett(conv lettere[]){
	uint8_t i;

	for(i=0; i<CARATTERI; i++){
		lettere[i].count=0;
		lettere[i].exist=-1;
	}
	return;
}

void inizial_PP(uint64_t pipe[],char plus[],uint8_t k){
	uint8_t i;

	for(i=0; i<k; i++){
		pipe[i]=0;
		plus[i]='\0';
	}
	return;
}

tree * primi_elementi(tree * root, char parola[],uint8_t k,int * elem){
	tree * new;

	if(scanf("%s",parola));
	while(parola[0]!='+'){//il + è quello di +nuova_partita o di +inserisci_fine
		new=malloc(sizeof(tree)+(k+1));
		if(new){
			new->sx=NULL;
			new->dx=NULL;
			new->p=NULL;
			strcpy(new->word,parola);
			root=inserisci(root,new);
			if(scanf("%s",parola));
			*(elem)=*(elem)+1;
		}else{
			printf("Errore su new\n");
		}
	}

	return root;
}

elem_t * nuovi_list(elem_t * head, char parola[],int * p_count,uint8_t k, conv lettere[],uint64_t pipe[],char plus[], uint8_t istogramma[]){
	int valid=1;
	uint64_t prov;
	int8_t i,j;

	i=0;
	j=CARATTERI;
	while(parola[i]!='\0' && valid==1){
		if(parola[i]!=parola[i-1] || i==0)
			j=vettpos(parola[i]);

		// controllo pipe
		prov=pipe[i]>>(CARATTERI-j-1) & 1;
			
		if(prov==1)
			valid=0;

		// controllo plus
		else if(plus[i]!='\0' && parola[i]!=plus[i]){
			valid=0;

		//controllo slash	
		}else if (lettere[j].exist==0)
			valid=0;

		i++;
	}

	//controllo count exist e count exactly
	creo_ist(parola,istogramma);
	i=0;
	while(i<CARATTERI && valid==1){
		if(lettere[i].exist!=-1){
			if(lettere[i].exist==1 && istogramma[i]<lettere[i].count)
				valid=0;
			else if(lettere[i].exist==2 && istogramma[i]!=lettere[i].count)
				valid=0;
		}
		i++;
	}
		
	if(valid==1){
		*(p_count)=*(p_count)+1;
		head=push(head,parola,k);
	}

	return head;
}

tree * nuovi_tree(tree * root, char parola[],uint8_t k){
	tree * new;

	new=malloc(sizeof(tree)+(k+1));
	if(new){
		new->sx=NULL;
		new->dx=NULL;
		new->p=NULL;
		strcpy(new->word,parola);
		root=inserisci(root,new);
		
	}else{
		printf("Errore\n");
	}

	return root;
}

void creo_ist(char parola[],uint8_t istogramma[]){
	int8_t i;
	int8_t j;

	for(i=0; i<CARATTERI; i++){
		istogramma[i]=0;
	}

	for(i=0; parola[i]!='\0'; i++){
			j=vettpos(parola[i]);
			istogramma[j]++;
	}

	return;
}

tree * inserisci(tree * root,tree * new){
	tree * pre;
	tree * cur;
	int ord;
	pre=NULL;
	cur=root;
	while(cur!=NULL){
		pre=cur;
		ord=ord_less(new->word,cur->word);
		if(ord==1)
			cur=cur->sx;
		else if(ord==0)
			cur=cur->dx;
	}
	new->p=pre;
	if(pre==NULL){
		root=new;
		root->color='b';
		return root;
	}
	ord=ord_less(new->word,pre->word);//provo a toglierlo
	if(ord==1){
		pre->sx=new;
	}else if(ord==0){
		pre->dx=new;
	}
	new->dx=NULL;
	new->sx=NULL;
	new->color='r';
	if(pre->color=='r')
		root=riparaRBinserisci(root,new);
	return root;
}


int ord_less(char a[],char b[]){
	int8_t i;
	i=0;
	while(a[i]!='\0' && a[i]==b[i])
		i++;
	if(a[i]>b[i])
		return 0;
	else if(a[i]<b[i])
		return 1;
	else
		return 2;
}

tree * riparaRBinserisci(tree * root,tree * z){
	tree * y;

	while(z->p!=NULL && z->p->color=='r'){
		if(z->p==z->p->p->sx){
			y=z->p->p->dx;
			if(y!=NULL && y->color=='r'){//caso 1
				z->p->color='b';
				y->color='b';
				z->p->p->color='r';
				z=z->p->p;
			}else{
				if(z==z->p->dx){//caso 2
					z=z->p;
					root=left_rotate(root,z);
				}
				z->p->color='b';//caso 3
				z->p->p->color='r';
				root=right_rotate(root,z->p->p);
			}
			 
		}else{//caso speculare alle slide
			y=z->p->p->sx;
			if(y!=NULL && y->color=='r'){//caso 1
				z->p->color='b';
				y->color='b';
				z->p->p->color='r';
				z=z->p->p;
			}else{
				if(z==z->p->sx){//caso 2
					z=z->p;
					root=right_rotate(root,z);
				}
				z->p->color='b';//caso 3
				z->p->p->color='r';
				root=left_rotate(root,z->p->p);
			}	
			
		}
	}

	root->color='b';
	return root;
}

tree * left_rotate(tree * root,tree * x){
	tree * y;

	y=x->dx;
	x->dx=y->sx;
	if(y->sx!=NULL)
		y->sx->p=x;
	y->p=x->p;
	if(x->p==NULL)
		root=y;
	else if(x==x->p->sx)
		x->p->sx=y;
	else
		x->p->dx=y;
	y->sx=x;
	x->p=y;
	return root;
}

tree * right_rotate(tree * root,tree * x){
	tree * y;

	y=x->sx;
	x->sx=y->dx;
	if(y->dx!=NULL)
		y->dx->p=x;
	y->p=x->p;
	if(x->p==NULL)
		root=y;
	else if(x==x->p->dx)
		x->p->dx=y;
	else
		x->p->sx=y;
	y->dx=x;
	x->p=y;
	return root;
}

char rev_vettpos(int8_t i){
	char c;

	if (i==0)
		c='-';
	if (i==37)
		c='_';
	if(i>=1 && i<=10)
		c = (char)(i+47);
	if(i>=11 && i<=36)
		c = (char)(i+54);
	if(i>=38 && i<=63)
		c = (char)(i+59);

	return c;
}

int8_t vettpos(char c){
	int8_t i;

	if (c=='-')
		i= 0;
	else if (c=='_')
		i= 37;
	else{
		i=(int)c;
		if(i>=48 && i<=57)
			i= i-47;
		if(i>=65 && i<=90)
			i= i-54;
		if(i>=97 && i<=122)
			i= i-59;
	}
	

	return i;
}

int exist(char parola[],tree * root){
	tree * tmp;
	int ord;

	tmp=root;
	ord=ord_less(parola,tmp->word);
	while(tmp!=NULL && ord!=2){
		if(ord==1)
			tmp=tmp->sx;
		else
			tmp=tmp->dx;

		if(tmp!=NULL)
			ord=ord_less(parola,tmp->word);
	}
	
	if(ord!=2)
		ord=0;

	return ord;
}

void confronti(char rif[],char rif2[],uint8_t istogramma[],char parola[],dvect parola2[],char output[]){
	int8_t i;
	int8_t j;
	int8_t idx;
	int8_t k;
	int8_t sum;
	int8_t count;
	//ordino vettore parola (counting sort stabile)
	for(i=0; rif[i]!='\0'; i++){
		if(rif[i]==parola[i]){
			output[i]='+';
		}else{
			output[i]='/';
		}
		rif2[i]='-';
		parola2[i].let='-';
		parola2[i].pos=255;//posizione non valida (mi aspetto parole meno lunghe di 255)
	}
	rif2[i]='\0';
	parola2[i].let='\0';
	output[i]='\0';
	k=i;

	creo_ist(parola,istogramma);

	sum=0;
	for(i=0; i<CARATTERI; i++){
		sum=sum+istogramma[i];
		istogramma[i]=sum;
	}

	count=0;
	for(i=k-1; i>=0;i--){
		j=vettpos(parola[i]);
		idx=istogramma[j];
		parola2[idx-1].let=parola[i];
		parola2[idx-1].pos=i;
		istogramma[j]=istogramma[j]-1;
		count++;
		
	}
	parola2[count].let='\0';
	//ordino vettore rif (counting sort normale)
	
	for(i=0; i<CARATTERI; i++){
		istogramma[i]=0;
	}

	for(i=0; rif[i]!='\0'; i++){
		if(output[i]!='+'){
			j=vettpos(rif[i]);
			istogramma[j]++;
		}
	}

	idx=0;
	j=0;
	for(i=0; i<CARATTERI; i++){
		while(istogramma[i]>0){
				rif2[idx]=rev_vettpos(i);
				idx++;
				istogramma[i]--;
		}
	}
	rif2[idx]='\0';
	//confornto 2 vettori e finisco di generare output

	i=0;
	j=0;
	while(rif2[j]!='\0' &&  parola2[i].let!='\0'){
		
		if(output[parola2[i].pos]!='+'){
			if(rif2[j]==parola2[i].let){
				output[parola2[i].pos]='|';
				i++;
				j++;
			}else if(rif2[j]>parola2[i].let){
				i++;
			}else{
				j++;
			}
		}else{
			i++;
		}
	}

	return ;
}

void delete(tree * root){
	if(root==NULL)
		return;
	else{
		delete(root->sx);
		delete(root->dx);
		free(root);
		return;
	}

}

void cambiamenti(uint64_t pipe[],char plus[],conv lettere[],dvect parola2[],char output[],int8_t pos_nuove[],int8_t ist_nuove[]){
	int8_t i;
	int8_t j;
	int8_t f,g;
	int8_t sum;
	int8_t slash;
	uint64_t prov;


	f=0;
	g=0;
	sum=0;
	slash=0;
	j=CARATTERI;
	for(i=0; parola2[i].let!='\0'; i++){
		if(parola2[i].let!=parola2[i-1].let && i!=0){
			if(sum>0 && slash>0 && lettere[j].exist!=2){
				// la parola c'è esattamente un tot di volte
				lettere[j].exist=2;
				if(sum>lettere[j].count){
					lettere[j].count=sum;
				}
				ist_nuove[f]=j;
				f++;
			}else if(sum>lettere[j].count && slash==0 && lettere[j].exist!=2){
				// la parola c'è un tot di volte
				if(lettere[j].exist==-1)
					lettere[j].exist=1;
				lettere[j].count=sum;
				ist_nuove[f]=j;
				f++;
			}else if(sum==0 && slash>0 && lettere[j].exist==-1){
				// miglioro slash
				lettere[j].exist=0;
				ist_nuove[f]=j;
				f++;
			}

			j=vettpos(parola2[i].let);
			sum=0;
			slash=0;
		}else if(i==0){
			j=vettpos(parola2[i].let);
		}


		if(output[parola2[i].pos]=='/'){
			slash ++;
			prov=1;
			prov=prov<<(CARATTERI-j-1);

			if((prov & pipe[parola2[i].pos]) == 0 && lettere[j].exist!=0){
				pipe[parola2[i].pos]=pipe[parola2[i].pos] | prov;
				pos_nuove[g]=parola2[i].pos;
				g++;
			}
		}else if(output[parola2[i].pos]!='/'){
				sum++;
				

			//miglioro plus
			if(output[parola2[i].pos]=='+' && plus[parola2[i].pos]=='\0'){
				plus[parola2[i].pos]=parola2[i].let;
				pos_nuove[g]=parola2[i].pos;
				g++;

			//miglioro pipe
			}else if(output[parola2[i].pos]=='|'){

				prov=1;
				prov=prov<<(CARATTERI-j-1);

				if((prov & pipe[parola2[i].pos]) == 0){
					pipe[parola2[i].pos]=pipe[parola2[i].pos] | prov;
					pos_nuove[g]=parola2[i].pos;
					g++;
				}
				
			}

		}

	}
	if(sum>0 && slash>0 && lettere[j].exist!=2){
		// la parola c'è esattamente un tot di volte
		lettere[j].exist=2;
		if(sum>lettere[j].count){
			lettere[j].count=sum;
		}
		ist_nuove[f]=j;
		f++;
	}else if(sum>lettere[j].count && slash==0 && lettere[j].exist!=2){
		// la parola c'è un tot di volte
		if(lettere[j].exist==-1)
			lettere[j].exist=1;
		lettere[j].count=sum;
		ist_nuove[f]=j;
		f++;
	}else if(sum==0 && slash>0 && lettere[j].exist==-1){
		// miglioro slash
		lettere[j].exist=0;
		ist_nuove[f]=j;
		f++;

	}

	ist_nuove[f]=-1;
	pos_nuove[g]=-1;

	return;
}

elem_t * camb_tree(tree * root,int * count,conv lettere[],char parola[],char output[],int8_t pos_nuove[],int8_t ist_nuove[],uint8_t istogramma[], elem_t * head,uint8_t k){
	int8_t i;
	int8_t j;
	int8_t valid=1;

	if(root!=NULL){
		i=0;
		j=0;
		creo_ist(root->word,istogramma);
		while((pos_nuove[i]!=-1 || ist_nuove[j]!=-1) && valid==1){
			if(pos_nuove[i]!=-1){
				if(output[pos_nuove[i]]=='+' && parola[pos_nuove[i]]!=root->word[pos_nuove[i]]){
					valid=0;
					*(count)=*(count)-1;
				}else if(output[pos_nuove[i]]!='+' && parola[pos_nuove[i]]==root->word[pos_nuove[i]]){
					valid=0;
					*(count)=*(count)-1;
				}
				i++;
			}


			if(ist_nuove[j]!=-1 && valid==1){
				if(lettere[ist_nuove[j]].exist==2 && lettere[ist_nuove[j]].count != istogramma[ist_nuove[j]]){
					valid=0;
					*(count)=*(count)-1;
				}else if(lettere[ist_nuove[j]].exist==1 && lettere[ist_nuove[j]].count > istogramma[ist_nuove[j]]){
					valid=0;
					*(count)=*(count)-1;
				}else if(lettere[ist_nuove[j]].exist==0 && istogramma[ist_nuove[j]]!=0){
					valid=0;
					*(count)=*(count)-1;
				}
				j++;
			}

		}

		if(valid==1)
			head=push(head,root->word,k);


		head=camb_tree(root->sx,count,lettere,parola,output,pos_nuove,ist_nuove,istogramma,head,k);
		head=camb_tree(root->dx,count,lettere,parola,output,pos_nuove,ist_nuove,istogramma,head,k);

	}
	return head;
}


elem_t * canc(elem_t * tmp){
	elem_t * del;

	del = tmp->next;
	tmp->next = tmp->next->next;
	free(del);

	return tmp;
}

elem_t * pop(elem_t * h){
	elem_t * del;

	del = h;
	h = h->next;
	free(del);

	return h;
}


void print(elem_t * h){
	elem_t * tmp;

	tmp = h;
	while(tmp != NULL){
		printf("%s\n", tmp->word);
		tmp = tmp->next;
	}
	return;
}

elem_t * push(elem_t * h,char seq[],uint8_t k){
	elem_t * tmp;

	tmp = malloc(sizeof(elem_t)+(1+k));

	if(tmp){
		strcpy(tmp->word,seq);

		tmp->next = h;
		h = tmp;
	}else{
		printf("push: errore allocazione memoria (%s)", seq);
	}

	return h;
}


elem_t * mergesort(elem_t * h,int count){
	elem_t * tmp=NULL;
	elem_t * h2=NULL;
	int i;

	if(count>2){
		tmp = h;
		i=count>>1;
		i--;
		while(i>0){
			tmp=tmp->next;
			i--;
		}
		h2=tmp->next;
		tmp->next=NULL;
		h=mergesort(h,count>>1);
		if(count%2==0)
			h2=mergesort(h2,count>>1);
		else
			h2=mergesort(h2,(count>>1)+1);


		h=merge(h,h2);

	}else if(count==2){
		if(ord_less(h->word,h->next->word)==0){
			tmp=h;
			h=h->next;
			h->next=tmp;
			h->next->next=NULL;
		}
	}

	return h;
}

elem_t * merge(elem_t * h1, elem_t * h2){
	elem_t * h=NULL;
	elem_t * tem=NULL;
	
	if(ord_less(h1->word,h2->word)==1){
		h=h1;
		h1=h1->next;
	}else{
		h=h2;
		h2=h2->next;
	}
	tem=h;
	while(h1!=NULL && h2!=NULL){
		if(ord_less(h1->word,h2->word)==1){
			tem->next=h1;
			h1=h1->next;
		}else{
			tem->next=h2;
			h2=h2->next;
		}
		tem=tem->next;
	}

	if(h2==NULL){
		tem->next=h1;
	}else{
		tem->next=h2;
	}

	return h;
}

elem_t * camb_list(elem_t * head,int * count,conv lettere[],char parola[],char output[],int8_t pos_nuove[],int8_t ist_nuove[],uint8_t istogramma[]){
	int8_t i;
	int8_t j;
	int8_t valid=0;

	elem_t * tmp=NULL;

	while(head!=NULL && valid==0){
		i=0;
		j=0;
		valid=1;
		creo_ist(head->word,istogramma);
		while((pos_nuove[i]!=-1 || ist_nuove[j]!=-1) && valid==1){
			if(pos_nuove[i]!=-1){
				if(output[pos_nuove[i]]=='+' && parola[pos_nuove[i]]!=head->word[pos_nuove[i]]){
					valid=0;
					*(count)=*(count)-1;
				}else if(output[pos_nuove[i]]!='+' && parola[pos_nuove[i]]==head->word[pos_nuove[i]]){
					valid=0;
					*(count)=*(count)-1;
				}
				i++;
			}


			if(ist_nuove[j]!=-1 && valid==1){
				if(lettere[ist_nuove[j]].exist==2 && lettere[ist_nuove[j]].count != istogramma[ist_nuove[j]]){
					valid=0;
					*(count)=*(count)-1;
				}else if(lettere[ist_nuove[j]].exist==1 && lettere[ist_nuove[j]].count > istogramma[ist_nuove[j]]){
					valid=0;
					*(count)=*(count)-1;
				}else if(lettere[ist_nuove[j]].exist==0 && istogramma[ist_nuove[j]]!=0){
					valid=0;
					*(count)=*(count)-1;
				}
				j++;
			}

		}
		
		if(valid==0)
			head=pop(head);
	}


	if(head!=NULL && head->next!=NULL){
		tmp=head;
		while(tmp->next!=NULL){
			i=0;
			j=0;
			valid=1;
			creo_ist(tmp->next->word,istogramma);
			while((pos_nuove[i]!=-1 || ist_nuove[j]!=-1) && valid==1){
				if(pos_nuove[i]!=-1){
					if(output[pos_nuove[i]]=='+' && parola[pos_nuove[i]]!=tmp->next->word[pos_nuove[i]]){
						valid=0;
						*(count)=*(count)-1;
					}else if(output[pos_nuove[i]]!='+' && parola[pos_nuove[i]]==tmp->next->word[pos_nuove[i]]){
						valid=0;
						*(count)=*(count)-1;
					}
					i++;
				}


				if(ist_nuove[j]!=-1 && valid==1){
					if(lettere[ist_nuove[j]].exist==2 && lettere[ist_nuove[j]].count != istogramma[ist_nuove[j]]){
						valid=0;
						*(count)=*(count)-1;
					}else if(lettere[ist_nuove[j]].exist==1 && lettere[ist_nuove[j]].count > istogramma[ist_nuove[j]]){
						valid=0;
						*(count)=*(count)-1;
					}else if(lettere[ist_nuove[j]].exist==0 && istogramma[ist_nuove[j]]!=0){
						valid=0;
						*(count)=*(count)-1;
					}
					j++;
				}

			}

			if(valid==0){
				tmp=canc(tmp);
			}else{
				tmp=tmp->next;
			}
		}
	}
	
	return head;
}

void stampa_tree(tree * root){
	if(root->sx!=NULL)
		stampa_tree(root->sx);
	printf("%s\n",root->word);
	if(root->dx!=NULL)
		stampa_tree(root->dx);
	return;
}