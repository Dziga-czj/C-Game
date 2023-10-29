#include  "../headers/dungeon.h"
#include "../headers/wall.h"

int compte_salles(Dungeon *dungeon)
{
    int s = 0;
    for (int i = 0; i < dungeon->size; i++)
    {
        for (int j = 0; j < dungeon->size; j++)
        {
            if(dungeon->map[i][j].type == 'e') s+=1;
        }
    }
    return s;
}

bool ouvertures(int x, int y, Dungeon *dungeon, int i, int j)
{
    if(((y == 10) || (y == 11)) && (x == 21) && dungeon->map[i][j].ouv_d) return false;
    if(((y == 10) || (y == 11)) && (x == 0) && dungeon->map[i][j].ouv_g) return false;
    if(((x == 10) || (x == 11)) && (y == 0) && dungeon->map[i][j].ouv_h) return false;
    if(((x == 10) || (x == 11)) && (y == 21) && dungeon->map[i][j].ouv_b) return false;

    return true;
}

void delete_dungeon(Dungeon *dungeon, WArray *warray)
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            dungeon->map[i][j].type = 'v';
        }
    }
    delete_walls(warray, 1);
}

void generate_dungeon(WArray *warray, Dungeon *dungeon)
{
    create_map(dungeon, 3, 3, 'o', 0);
    int d_x = -160+36;
    int d_y = -160+34;
    for (int i = 0; i < dungeon->size; i++)
    {
        for (int j = 0; j < dungeon->size; j++)
        {
            if(dungeon->map[i][j].type == 'e' || dungeon->map[i][j].type == 'd')
            {
                int r = rand () % (dungeon->nb_rooms-1)+1;

                if(dungeon->map[i][j].type == 'd') r = 0;

                for (int x = 0; x < SIZE; x++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        
                        if(dungeon->room_mesh[r][x][y] != '_')
                        {
                            if(ouvertures(x, y, dungeon, i, j)) //check les ouvertures pour les salles
                            {   
                                addWall(warray, i*(SIZE+16)+x+d_x, j*(SIZE+16)+y+d_y, 1, 1, 1, dungeon->room_mesh[r][x][y]);
                            }
                            
                        }

                    }
                }

                if(dungeon->map[i][j].ouv_d)
                {
                    addWall(warray, i*(SIZE+16)+22+d_x, j*(SIZE+16)+9+d_y, 8, 1, 1, 'h');
                    addWall(warray, i*(SIZE+16)+22+d_x, j*(SIZE+16)+12+d_y, 8, 1, 1, 'b');
                }
                if(dungeon->map[i][j].ouv_g)
                {
                    addWall(warray, i*(SIZE+16)-8+d_x, j*(SIZE+16)+9+d_y, 8, 1, 1, 'h');
                    addWall(warray, i*(SIZE+16)-8+d_x, j*(SIZE+16)+12+d_y, 8, 1, 1, 'b');
                }
                if(dungeon->map[i][j].ouv_h)
                {
                    addWall(warray, i*(SIZE+16)+9+d_x, j*(SIZE+16)-8+d_y, 1, 8, 1,'g');
                    addWall(warray, i*(SIZE+16)+12+d_x, j*(SIZE+16)-8+d_y, 1, 8, 1, 'd');
                }
                if(dungeon->map[i][j].ouv_b)
                {
                    addWall(warray, i*(SIZE+16)+9+d_x, j*(SIZE+16)+22+d_y, 1, 8, 1,'g');
                    addWall(warray, i*(SIZE+16)+12+d_x, j*(SIZE+16)+22+d_y, 1, 8, 1,'d');
                
                }

            }

        }
    }
    //printf("used : %d", warray->used);
}

void get_rooms(Dungeon *dungeon)
{
    FILE *fp = fopen("ressources/text_files/rooms.txt", "r");
    //int ndata = 0;

    int x = -1;
    int counter = 0;
    int nb = 0; 
    while(!feof(fp))
    {
        
        if(x == -1){
            fscanf(fp, "n = %d\n", &nb);
            dungeon->nb_rooms = nb;
            if((dungeon->room_mesh = malloc(nb * sizeof *dungeon->room_mesh)) == NULL){
                perror("malloc 1");
            }

            for (int i = 0; i < nb; i++)
                dungeon->room_mesh[i] = NULL;
            
            for (int i = 0; i < nb; i++)
                if((dungeon->room_mesh[i] = malloc(SIZE * sizeof *dungeon->room_mesh[i])) == NULL){
                    for (int j = 0; j < i; j++)
                        free(dungeon->room_mesh[j]);
                    perror("malloc 2");
                }

            for (int i = 0; i < nb; i++)
                for (int j = 0; j < SIZE; j++)
                    dungeon->room_mesh[i][j] = NULL;

            for (int i = 0; i < nb; i++)
                for (int j = 0; j < SIZE; j++)
                    if((dungeon->room_mesh[i][j] = malloc(SIZE * sizeof *dungeon->room_mesh[i][j])) == NULL){
                        for (int x = 0; x < i; x++)
                            for (int y = 0; y < j; y++)
                                free(dungeon->room_mesh[x][y]);
                        perror("malloc 3");
                    }
            x = 0;
        }
        else
        {
            if(x < nb)
            {
                char ligne[SIZE];
                fscanf(fp, "%s\n", &ligne[0]);
                //printf("getting %d : %d\n", x, counter);
                for (int i = 0; i < SIZE; i++)
                {
                    dungeon->room_mesh[x][i][counter] = ligne[i];
                    //printf("%c",ligne[i]);
                }
                //printf("\n");
                counter++;
            }

            if(counter == SIZE)
            {
                x++;
                counter = 0;
            }




        }
        //ndata++;
    }
    
}

Dungeon create_dungeon()
{
    Dungeon dungeon;
    dungeon.size = 7;

    for(int i = 0; i < dungeon.size; i++)
    {
        for(int j = 0; j < dungeon.size; j++)
        {
            dungeon.map[i][j].type = 'v';
        }
    }
    get_rooms(&dungeon);

    return dungeon;
}


void print_dungeon(Dungeon *dungeon)
{
    for (int i = 0; i < dungeon->size; i++)
    {
        for (int j = 0; j < dungeon->size; j++)
        {
            if(dungeon->map[j][i].type == 'v') printf(" ");
            else printf("%c", dungeon->map[j][i].type);
        }
        printf("\n");
    }
}

bool est_dans(int x, int y)
{
    if (x < 0 ) return false;
    if (x >= 7 ) return false;
    if (y < 0 ) return false;
    if (y >= 7 ) return false;
    else return true;
}

bool est_room(Dungeon *dungeon, int x, int y)
{
    if(dungeon->map[x][y].type == 'e') return true;
    else return false;
}

void create_map(Dungeon *dungeon, int x, int y, char dir, int n)
{
    int r = 0;
    if(n == 0)
    {
        dungeon->map[x][y].type = 'd';
        r = rand() % 4;
        dungeon->map[x][y].ouv_d = (r != 0);
        dungeon->map[x][y].ouv_g = (r != 1);
        dungeon->map[x][y].ouv_b = (r != 2);
        dungeon->map[x][y].ouv_h = (r != 3);

        if(r != 0) create_map(dungeon, x+1, y, 'd', 1+(rand()%2));
        if(r != 1) create_map(dungeon, x-1, y, 'g', 1+(rand()%2));
        if(r != 2) create_map(dungeon, x, y+1, 'b', 1+(rand()%2));
        if(r != 3) create_map(dungeon, x, y-1, 'h', 1+(rand()%2));
       
    }else if (n == 1)
    {
        dungeon->map[x][y].type = 'e';
        dungeon->map[x][y].ouv_d = false;
        dungeon->map[x][y].ouv_g = false;
        dungeon->map[x][y].ouv_h = false;
        dungeon->map[x][y].ouv_b = false;


        if(dir == 'g'){
            dungeon->map[x][y].ouv_d = true;
            dungeon->map[x][y].ouv_g = true;
            create_map(dungeon, x-1, y, 'g', n+1);
        } 
        else if(dir == 'b'){
            dungeon->map[x][y].ouv_b = true;
            dungeon->map[x][y].ouv_h = true;
            create_map(dungeon, x, y+1, 'b', n+1);
        } 
        else if(dir == 'h'){
            dungeon->map[x][y].ouv_b = true;
            dungeon->map[x][y].ouv_h = true;
            create_map(dungeon, x, y-1, 'h', n+1);
        } 
        else{
            dungeon->map[x][y].ouv_d = true;
            dungeon->map[x][y].ouv_g = true;
            create_map(dungeon, x+1, y, 'd', n+1);
        } 

    }
    else if (n<=3)
    {
        dungeon->map[x][y].type = 'e';
        r = rand() % 3;

        dungeon->map[x][y].ouv_d = false;
        dungeon->map[x][y].ouv_g = false;
        dungeon->map[x][y].ouv_h = false;
        dungeon->map[x][y].ouv_b = false;

        if(dir == 'h')
        {
            dungeon->map[x][y].ouv_b = true;
            if(r != 0 && est_dans(x+1,y) && !est_room(dungeon, x+1,y)){
                if (n != 3)
                dungeon->map[x][y].ouv_d = true;
                create_map(dungeon, x+1, y, 'd', n+1);
            } 
            if(r != 1 && est_dans(x-1,y) && !est_room(dungeon, x-1,y)){
                if (n != 3)
                dungeon->map[x][y].ouv_g = true;
                create_map(dungeon, x-1, y, 'g', n+1);
            } 
            if(r != 2 && est_dans(x,y-1) && !est_room(dungeon, x,y-1)){
                if (n != 3)
                dungeon->map[x][y].ouv_h = true;
                create_map(dungeon, x, y-1, 'h', n+1);
            } 
        }   
        else if(dir == 'b')
        {
            dungeon->map[x][y].ouv_h = true;
            if(r != 0 && est_dans(x+1,y) && !est_room(dungeon, x+1,y)){
                if (n != 3)
                dungeon->map[x][y].ouv_d = true;
                create_map(dungeon, x+1, y, 'd', n+1);
            } 
            if(r != 1 && est_dans(x-1,y) && !est_room(dungeon, x-1,y)){
                if (n != 3)
                dungeon->map[x][y].ouv_g = true;
                create_map(dungeon, x-1, y, 'g', n+1);
            } 
            if(r != 2 && est_dans(x,y+1) && !est_room(dungeon, x,y+1)){
                if (n != 3)
                dungeon->map[x][y].ouv_b = true;
                create_map(dungeon, x, y+1, 'b', n+1);
            } 

        }
        else if(dir == 'g')
        {
            dungeon->map[x][y].ouv_d = true;
            if(r != 0 && est_dans(x-1,y) && !est_room(dungeon, x-1,y)){
                if (n != 3)
                dungeon->map[x][y].ouv_g = true;
                create_map(dungeon, x-1, y, 'g', n+1);
            } 
            if(r != 1 && est_dans(x,y-1) && !est_room(dungeon, x,y-1)){
                if (n != 3)
                dungeon->map[x][y].ouv_h = true;
                create_map(dungeon, x, y-1, 'h', n+1);
            } 
            if(r != 2 && est_dans(x,y+1) && !est_room(dungeon, x,y+1)){
                if (n != 3)
                dungeon->map[x][y].ouv_b = true;
                create_map(dungeon, x, y+1, 'b', n+1);
            } 

        }
        else // 'd'
        { 
            dungeon->map[x][y].ouv_g = true;
            if(r != 0 && est_dans(x+1,y) && !est_room(dungeon, x+1,y)){
                if (n != 3)
                dungeon->map[x][y].ouv_d = true;
                create_map(dungeon, x+1, y, 'd', n+1);
            } 
            if(r != 1 && est_dans(x,y-1) && !est_room(dungeon, x,y-1)){
                if (n != 3)
                dungeon->map[x][y].ouv_h = true;
                create_map(dungeon, x, y-1, 'h', n+1);
            }   
            if(r != 2 && est_dans(x,y+1) && !est_room(dungeon, x,y+1)){
                if (n != 3)
                dungeon->map[x][y].ouv_b = true;
                create_map(dungeon, x, y+1, 'b', n+1);
            } 

        }
    }
}




