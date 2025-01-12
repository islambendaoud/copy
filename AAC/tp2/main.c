#include <stdio.h>

static int* tab;
static int cpt;

struct conf{
  int i;
  int j;
  int m;
  int n;
};





/*Inverse la plaque de choco_stade_unlat si elle plus haute que large.
 le choix du type de rotation est arbitraire, 
l'objectif est juste de transformer toute les plaques en plaque plus large que haute.*/
struct conf simpli_conf(int i,int j, int m, int n){

  int ii=i,jj=j,mm=m,nn=n;
  struct conf cf;

  if(i<j){
    ii=j;
    jj=i;
    mm=n;
    nn=m;}
  //printf("�tat simplifi� de\ni:%d j:%d m:%d n:%d\ni:%d j:%d m:%d n:%d\n\n",i,j,m,n,ii,jj,mm,nn);
  cf.i=ii;
  cf.j=jj;
  cf.m=mm;
  cf.n=nn;
  return cf;
}


/* en plus de rendre la plaque plus large que haute, cette fonction va d�placer le choco_stade_unlat de la mort
   pour qu'il soit positionn� le plus en haut a gauche possible tout en conservant la meme issue au jeu.
   On pose la plaque sur une table, et l'on imagine la face visible,(a) et la face cache(b).
   En passant de la face a � la face b en effectuant un retournant de la plaquette( tourner la page d'un livre), on change la position du choco_stade_unlat sans changer l'issue du jeu. En jouant avec les differents retournements, on peut optimiser une configuration
   en une configuration plus large que haute, avec le choco_stade_unlat de la mort le plus en haut a gauche possible.ainsi; 3100 a la meme issue que 3101 3110 3111  1300 1301 1310 1311*/
struct conf opti_conf(int i,int j, int m, int n){
  struct conf cf;
  int ii=i,jj=j,mm=m,nn=n;

  if(i<j){
    ii=j;
    jj=i;
    mm=n;
    nn=m;}

  if(mm+1>(ii/2+(ii%2)))
    mm=(ii-(mm+1));
  if(nn+1>(jj/2+(jj%2)))
    nn=(jj-(nn+1));

  // printf("�tat simplifi� de\ni:%d j:%d m:%d n:%d\ni:%d j:%d m:%d n:%d\n\n",i,j,m,n,ii,jj,mm,nn);
  cf.i=ii;
  cf.j=jj;
  cf.m=mm;
  cf.n=nn;
  return cf;
}

int choco_stade_trois(int i,int j,int m, int n){
  //printf("entr� dans choco\n");

  struct conf cf;
  int io,jo,mo,no;
  cf =opti_conf(i,j,m,n);
  //printf("i:%d , j:%d , m:%d , n:%d , cfi:%d , cfj:%d , cfm:%d , cfn:%d\n",i,j,m,n,cf.i,cf.j,cf.m,cf.n);
  i=cf.i;
  j=cf.j;
  m=cf.m;
  n=cf.n;


  //  printf("Avant traitement i:%d j:%d m:%d n:%d\n",i,j,m,n);
  if(i==1 && j==1){
    //printf("i:%d , j:%d , m:%d , n:%d donne 0\n",i,j,m,n);
    tab[0] =0;
    return 0;}

  int tl,th,bcl,bch,res,fres,commu; // commu permet de traiter le cas o� aucun r�sultat n'est enregistr�.
  commu=0;

  if((res=tab[full_opti_trans_conf_to_int(i,j,m,n)]) != -1){
    //printf(" deja vu: %d\n",res);
    return res;}


  // printf("for1:  %d:   %d   %d   %d  \n",i,j,m,n);
  for(th=(j),bch=(n);th>1 && bch>0;th--,bch--){ // on coupe par en haut
    cf =opti_conf(i,th,m,bch);
    io=cf.i;
    jo=cf.j;
    mo=cf.m;
    no=cf.n;
    if((res=tab[full_opti_trans_conf_to_int(io,jo,mo,no)]) == -1)
      res=choco_stade_trois(io,j-jo,mo,n-no);
    if(res==0){
      tab[full_opti_trans_conf_to_int(i,j,m,n)] = 1;
      return 1;}


    if(commu==0){
      fres=res;
      commu=1;}


    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;
  }


  for(tl=i,bcl=m;tl>1 && bcl>0;tl--,bcl--){ // on coupe par la gauche
    cf =opti_conf(tl,j,bcl,n);
    io=cf.i;
    jo=cf.j;
    mo=cf.m;
    no=cf.n;
    if((res=tab[full_opti_trans_conf_to_int(io,jo,mo,no)]) == -1)
      res=choco_stade_trois(i-io,jo,i-io,no);
    if(res==0){
      tab[full_opti_trans_conf_to_int(i,j,m,n)] = 1;
      return 1;}

    if(commu==0){
      fres=res;
      commu=1;}
    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;
  }

  // printf("i:%d j:%d m:%d n:%d  longueur:%d\n",i,j,m,n,fres);
  if(tab[full_opti_trans_conf_to_int(i,j,m,n)] == -1)
    tab[full_opti_trans_conf_to_int(i,j,m,n)] =fres+1;
  return fres+1;


}


int choco_stade_deux(int i,int j,int m, int n){



  //  printf("Avant traitement i:%d j:%d m:%d n:%d\n",i,j,m,n);
  if(i==1 && j==1)
    return 0;



  int tl,th,bcl,bch,res,fres,commu; // commu permet de traiter le cas o� aucun r�sultat n'est enregistr�.
  commu=0;


  // printf("for1:  %d:   %d   %d   %d  \n",i,j,m,n);
  for(th=(j),bch=(n);th>1 && bch>0;th--,bch--){ // on coupe par en haut
  
    if((res=tab[simple_opti_trans_conf_to_int(i,(th-1),m,(bch-1))]) == -1)
      res=choco_stade_deux(i,(th-1),m,(bch-1));



    if(commu==0){
      fres=res;
      commu=1;}


    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;
  }


  for(tl=i,bcl=m;tl>1 && bcl>0;tl--,bcl--){ // on coupe par la gauche
    if((res =tab[simple_opti_trans_conf_to_int(tl-1,j,bcl-1,n)]) == -1)
      res=choco_stade_deux(tl-1,j,bcl-1,n);

    if(commu==0){
      fres=res;
      commu=1;}
    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;
  }


  for(th=j;th>n+1;th--){// on coupe par en bas
    if((res =tab[ simple_opti_trans_conf_to_int(i,(th-1),m,n)]) == -1)
      res=choco_stade_deux(i,(th-1),m,n);

    if(commu==0){
      fres=res;
      commu=1;}
    if(fres%2==0){
      if(res%2==1)
	fres=res;
      else if(fres<res)
	fres=res;
    }
    else if( res%2==0 && res<fres)
      fres=res;
  }


  for(tl=i;tl>m+1;tl--){// on coupe par la droite
    if((res = tab[simple_opti_trans_conf_to_int(tl-1,j,m,n)]) == -1)
      res=choco_stade_deux(tl-1,j,m,n);

    if(commu==0){
      fres=res;
      commu=1;}
    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;


  }
  // printf("i:%d j:%d m:%d n:%d  longueur:%d\n",i,j,m,n,fres);
  if(tab[simple_opti_trans_conf_to_int(i,j,m,n)] == -1)
    tab[simple_opti_trans_conf_to_int(i,j,m,n)] =fres+1;
  return fres+1;


}




int choco_stade_un(int i,int j,int m, int n){



  //  printf("Avant traitement i:%d j:%d m:%d n:%d\n",i,j,m,n);
  if(i==1 && j==1)
    return 0;



  int tl,th,bcl,bch,res,fres,commu; // commu permet de traiter le cas o� aucun r�sultat n'est enregistr�.
  commu=0;


  // printf("for1:  %d:   %d   %d   %d  \n",i,j,m,n);
  for(th=(j),bch=(n);th>1 && bch>0;th--,bch--){ // on coupe par en haut
  
   
    res=choco_stade_un(i,(th-1),m,(bch-1));



    if(commu==0){
      fres=res;
      commu=1;}


    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;
  }


  for(tl=i,bcl=m;tl>1 && bcl>0;tl--,bcl--){ // on coupe par la gauche
    
    res=choco_stade_un(tl-1,j,bcl-1,n);

    if(commu==0){
      fres=res;
      commu=1;}
    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;
  }


  for(th=j;th>n+1;th--){// on coupe par en bas
    
    res=choco_stade_un(i,(th-1),m,n);

    if(commu==0){
      fres=res;
      commu=1;}
    if(fres%2==0){
      if(res%2==1)
	fres=res;
      else if(fres<res)
	fres=res;
    }
    else if( res%2==0 && res<fres)
      fres=res;
  }


  for(tl=i;tl>m+1;tl--){// on coupe par la droite
   
    res=choco_stade_un(tl-1,j,m,n);

    if(commu==0){
      fres=res;
      commu=1;}
    if(fres%2==0){// si les resultats jusque maintenant sont perdant
      if(res%2==1)// si res est gagnant
	fres=res;
      else if(fres<res)//si rez et perdant et plus grand que frez
	fres=res;
    }
    else if( res%2==1 && res<fres)//modif res%2==0
      fres=res;


  }
  // printf("i:%d j:%d m:%d n:%d  longueur:%d\n",i,j,m,n,fres);

  return fres+1;


}

/* l'objectif et de stocker 4 valeurs entieres en un seul entier.
   Cette fonction ne s'utilise que si la plaquette est plus large que haute.*/
struct conf simple_opti_trans_int_to_conf ( int val){
  struct conf cf;
  int bak=val;
  int i,j,m,n; // on commence par la conf la plus simple
  i=1;
  j=1;
  m=0;
  n=0;


  while(val>0){// tant que l'on est pas arriv� � la configuration voulue
    if(j==i && m==i-1 && n==j-1){ // si l'on a parcouru tout les cas pour une valeur de i donn�: 2211 par exemple
      val--;
      i++;
      j=1;
      n=0;
      m=0;
    }
    else if(m==i-1 && n==j-1 ){ // si l'on a parcouru tout les cas pour une valeur de i,j donn�: 3221 par exemple
      val--;
      j++;
      m=0;
      n=0;
    }
    else if( n==j-1 ){//si l'on a parcouru tout les cas pour une valeur de i,j,m donn� 4403 par exemple
      val--;
      n=0;
      m++;
    }
    else{ // pour tout les autres cas
      n++;
      val--;
    }
  }



  printf("�tat final pour %d: i:%d j:%d m:%d n:%d\n",bak,i,j,m,n);
  //printf("passage inverse :%d\n",trans(i,j,m,n));
  simpli_conf(i,j,m,n);
  cf.i=i;
  cf.j=j;
  cf.m=m;
  cf.n=n;
  return cf;
}

/*l'op�ration inverse de simple_opti_trans_int_to_conf*/

int simple_opti_trans_conf_to_int ( int i,int j,int m,int n){
  struct conf cf=simpli_conf(i,j,m,n);
  j=cf.j;
  i=cf.i;
  m=cf.m;
  n=cf.n;
  int val=0;

  while(!(j==1 && m==0 && n==0 && i==1)){
    if(j==1 && m==0 && n==0){
      val++;
      i--;
      j=i;
      n=j-1;
      m=i-1;
    }
    else if(m==0 && n==0 ){
      val++;
      j--;
      m=i-1;
      n=j-1;
    }
    else if( n==0){
      val++;
      n=j-1;
      m--;
    }
    else{
      n--;
      val++;
    }
  }
  return val;

}

/*Fonction qui stocke 4 valeur entiere dans un seul entier, pour des configuration non optimis�es. Elle ne
  sera pas utilis� mais elle est correcte. Elle est l� pour justifier la pr�sence des fonctions d'optimisation simple, facilitant
  grandement la manipulation des donn�es avant l'optimisation compl�te.*/
struct conf no_opti_trans_int_to_conf ( int val){
  struct conf cf;
  int bak=val;
  int tick=1;// tick permet de gerer l'altenance j>i -> i>j
  int tock=0;// tock permet d'empecher les doublons j==i qui s'il ne sont pas trait�, apparaissent deux fois.
  // quand i s'incremete jusqua j, et quand j s'incremente jusqua i.


  int i,j,m,n,max; // max repr�sente la valeur max atteinte par i ou j dans l'�xecution de la fonction
  i=1;
  j=1;
  m=0;
  n=0;
  max=2;


  /* Ignorons m et n qui sont trait� de la meme maniere dans toute les fonctions de conversion.
     cette fonction gere l�volution de i et j ainsi:
     j s'incr�mente jusqua i, j reste et i est reinitialis� , puis s'incr�mente jusqu'a j et enfin le cas ou i==j est consid�r�,
     puis l'on reinitialise j et l'on incr�mente i de 1*/

  while(val>0){
    if(j==i && ((m==i-1 && n==j-1)|| tock) && val>0){
      val--;



      if(tick){
	i=max;
	j=1;
	tick=0;
	if(tock)
	  val++;
	tock=1-tock;
      }
      else{
	j=max;
	i=1;
	tick=1;
	max++;
	tock=1-tock;

      }



      n=0;
      m=0;
    }


    else if(m==i-1 && n==j-1 && val>0){
      val--;
      if(!tick)
	j++;
      else
	i++;
      m=0;
      n=0;
      if(i==j && tock)
        val++;

    }




    else if( n==j-1 && val>0){
      val--;
      n=0;
      m++;
    }
    else{
      n++;
      val--;
    }
  }



  printf("�tat final pour %d: i:%d j:%d m:%d n:%d\n",bak,i,j,m,n);
  //printf("passage inverse :%d\n",trans(i,j,m,n));
  //opti_conf(i,j,m,n);
  cf.i=i;
  cf.j=j;
  cf.m=m;
  cf.n=n;
  return cf;
}

// donne une configuration en fonction de l'neiter  dans une situation
//ou le vecteur ne contient que des confioguration optimis�,
// c'est a dire toute les configuration stock� font reference a des tablette plus large que haute
// et le carr� de la mort est le plus en haut a gauche possible
struct conf full_opti_trans_int_to_conf ( int val){
  struct conf cf;
  cpt++;
  int bak=val;
  int i,j,m,n;
  i=1;
  j=1;
  m=0;
  n=0;


  while(val>0){
    if(j==i && m==((i/2)-1+i%2) && n==((j/2)-1+j%2) ){
      val--;
      i++;
      j=1;
      n=0;
      m=0;
    }
    else if(m==((i/2)-1+i%2) && n==((j/2)-1+j%2) ){
      val--;
      j++;
      m=0;
      n=0;
    }
    else if( n==((j/2)-1+j%2) ){
      val--;
      n=0;
      m++;
    }
    else{
      n++;
      val--;
    }
  }



  printf("�tat final pour %d: i:%d j:%d m:%d n:%d\n",bak,i,j,m,n);
  //printf("passage inverse :%d\n",trans5(i,j,m,n));
  //opti_conf(i,j,m,n);
  cf.i=i;
  cf.j=j;
  cf.m=m;
  cf.n=n;
  return cf;
}
//Prend n'importe quelle configuration et la stocke dans un entier. la configuration est d'abord optimis�.
int full_opti_trans_conf_to_int ( int i,int j,int m,int n){
  cpt++;
  struct conf cf=opti_conf(i,j,m,n);
  j=cf.j;
  i=cf.i;
  m=cf.m;
  n=cf.n;
  int val=0;

  while(!(j==1 && m==0 && n==0 && i==1)){
    if(j==1 && m==0 && n==0){
      val++;
      i--;
      j=i;
      n=((j/2)-1+j%2);
      m=((i/2)-1+i%2);
    }
    else if(m==0 && n==0 ){
      val++;
      j--;
      m=((i/2)-1+i%2);
      n=((j/2)-1+j%2);
    }
    else if( n==0){
      val++;
      n=((j/2)-1+j%2);
      m--;
    }
    else{
      n--;
      val++;
    }
  }
  return val;

}

void init_tab(int val){

  int i;
  for (i = 0; i <= val ; i++)
    tab[i] = -1;
  //for(i = 0;i < val; i ++)
  //printf("%d\n",tab[i]);


}



int main(int argc,char**argv){


  struct conf cf;
  int i,j;
  int valeur,v;








  /*cf = simpli_conf(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
    valeur = simple_opti_trans_conf_to_int (cf.i,cf.j,cf.m,cf.n);
    tab = malloc( (valeur +1) * sizeof(int) );
    init_tab(valeur);
    valeur = choco_stade_deux(cf.i,cf.j,cf.m,cf.n);
    printf("simpli_opti valeur: %d\n",valeur);*/

  

  cf = opti_conf(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
  valeur = full_opti_trans_conf_to_int (cf.i,cf.j,cf.m,cf.n);
  tab = malloc( (valeur +1) * sizeof(int) );
  init_tab(valeur);
  v = choco_stade_trois(cf.i,cf.j,cf.m,cf.n);
  printf("opti_opti valeur: %d   cpt:%d\n",v,cpt);
  
  /*for(i = 0;i < valeur; i ++){
    if(tab[i]!=-1)
    printf("tab[%d]: %d\n",i,tab[i]);
    }*/


  /* for (j=0;j<=atoi(argv[1]);j++){

  cf=full_opti_trans_int_to_conf(j);
  // i=trans5(cf.i,cf.j,cf.m,cf.n);
  // printf("reconst: %d\n",i);
  // printf("res: i:%d  j:%d  m:%d  n:%d\n",cf.i,cf.j,cf.m,cf.n);
  }*/



  /*
  i=trans5(100,100,50,50);
  printf("res: %d\n",i);
  cf=trans4(i);
  printf("reconstitution: i:%d  j:%d  m:%d  n:%d\n",cf.i,cf.j,cf.m,cf.n);
  */



  return 0;
}
