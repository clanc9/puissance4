#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	gecko_read(char *str)
{
  return (read(0, str, 2));
}

void my_putchar(char c)
{
      write(1,&c,1);
}

void my_show_str(char *str)
{
  int i;
  i = 0;
  while ( str[i] != '\0' )
	  {
	    my_putchar(str[i]);
	    i++;
	  }
  return;
}

void show_game(char **game_array)
{
  int	counter_y;
  counter_y = 0;

  printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
  printf("=============================\n");
  while (counter_y < 6)
  {
      printf("| %c | %c | %c | %c | %c | %c | %c |\n",
	     game_array[counter_y][0], game_array[counter_y][1],
	     game_array[counter_y][2], game_array[counter_y][3],
	     game_array[counter_y][4], game_array[counter_y][5],
	     game_array[counter_y][6]);
      counter_y++;
  }
  printf("\n");
}

// détermine si le joueur de jeton 'char c' est gagnant
int	succes(char c, char **game_array)
{
  int	counter_y, ret, i, j;
  i = 0;
  // vérifie alignement de 4 jetons à la verticale
  while ( (i < 6) && (ret == 0) )
  {
    counter_y = 5;
    while ( (counter_y >= 3) && (ret == 0) )
    {
	       ret = 1;   // variable de victoire.  0 = échec
	       for (j = 0; j<4; j++)
	       {
	           if (game_array[counter_y-j][i] != c)
	               ret = 0;
	       }
	       counter_y--;
    }
    i++;
  }
  // vérifie alignement de 4 jetons à la verticale
  counter_y = 5;
  while ( (counter_y >= 0) && (ret == 0) )
  {
    i = 0;
    while ( (i <= 3) && (ret == 0) )
    {
	       ret = 1;
	       for (j = 0; j<4; j++)
	       {
	           if (game_array[counter_y][i+j] != c)
	               ret = 0;
	       }
	       i++;
    }
  counter_y--;
  }

  // diagonale y = -ax
  // vérifie alignement de 4 jetons à la diagonale haut en bas
  counter_y = 0;
  while ( (counter_y <= 2) && (ret == 0) )
  {
    i = 0;
    while ( (i <= 3) && (ret == 0) )
    {
	        ret = 1;
	        for (j = 0; j<4; j++)
	        {
   	          if ( game_array[counter_y+j][i+j] != c)
	               ret = 0;
	        }
	        i++;
    }
    counter_y++;
  }

  // diagonale y = ax
  // vérifie alignement de 4 jetons à la diagonale bas en haut
  counter_y = 0;
  while ( (counter_y <= 2) && (ret == 0) )
  {
      i = 6;
      while ( (i >= 3) && (ret == 0) )
      {
	         ret = 1;
	         for (j = 0; j<4; j++)
	         {
   	            if ( game_array[counter_y+j][i-j] != c)
	              ret = 0;
	         }
	         i--;
       }
       counter_y++;
  }
return ret;
}

int	ajout(char c, int col, char **game_array)
{
  int	counter_y, ret;
  counter_y = 5;
  ret = 0;
  while (counter_y >= 0)
  {
      if ( (game_array[counter_y][col-1] == '-') && (ret == 0))
	    {
	         game_array[counter_y][col-1] = c;
	         ret = 1;
	    }
      counter_y--;
  }
  return ret;
}

int	main()
{
  int i, j, k, c, col, rangee, n_coup, n_coup_max, ok,  ok_reponse;
  int ret, ret2, re;
  char joueur;
  char **grille;
  char *choix;
  choix = malloc(2 * sizeof(char));
  char ch1[39] = "Felicitations joueur 1, vous gagnez !\n\n";
  char ch2[39] = "Felicitations joueur 2, vous gagnez !\n\n";
  char ch3[13] = "Match nul !\n\n";
  char playerone[23] = "Au tour du joueur 1 : ";
  char playertwo[23] = "Au tour du joueur 2 : ";

  col = 7;
  rangee = 6;
  n_coup_max = col * rangee + 1;

  // tableau à deux dimensions contenant les positions des jetons
  grille = malloc( (rangee + 1) * sizeof(char *) );
  for (i = 0; i<rangee; i++)
    grille[i] = malloc( (col+1) * sizeof(char) );

  // initialisation de la grille
  for (i = 0; i<rangee; i++)
  {
      for (j = 0; j<col; j++)
          grille[i][j] = '-';
  }

  ok = 0;   // variable prend la valeur 1 (true) s'il y a un gagnant
  n_coup = 1;
  joueur = 'X';
  while ((n_coup < n_coup_max) && (ok == 0))
    {
        ok_reponse = 0;
	      while (ok_reponse == 0)
	      {
	           show_game(grille);
	           if (joueur == 'X')
	               my_show_str(playerone);
	           else
	               my_show_str(playertwo);

	         re = gecko_read(choix);    // entrée de la colonne à joueur
	         k = atoi(choix);
	         if ( (k > 0) && (k < 8) )   // valide le choix
	         {
	             ret = ajout(joueur, k, grille);
		           if (ret == 1)
		              ok_reponse = 1;  // sort de la boucle si ajout effectué
	         }
	      my_putchar('\n');
	      }

	ret2 = succes(joueur, grille);
  // vérifie si le joueur qui vient d'ajouter un jeton gagne
	if (ret2 == 1)
	{
      show_game(grille);
	    if (joueur == 'X')
	      ok = 1;
	    else
	      ok = 2;
	}
  // change de joueur
	if (joueur == 'X')
	  joueur = 'O';
	else
	  joueur = 'X';
	n_coup++;
  }

  // affichage du message de fin selon la valeur de la variable ok
  if (ok == 1)
      my_show_str(ch1);
  else if (ok == 2)
      my_show_str(ch2);
  else
      my_show_str(ch3);

}
