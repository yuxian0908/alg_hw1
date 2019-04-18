#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <math.h>
#include <stdio.h>
using namespace std;

/*
流程：
    V1. 讀檔
    V2. 建立image array
    V3. 用bfs建立connected array 並計算數量
    V4. 根據selected pixel找到該connected component並將裡面所有點換成目標color 
    5. output
*/

struct triple
{
    int row;
    int col;
    char c;
    triple(int row, int col, char c):row(row),col(col),c(c){}
};


int main(){
    string image_file_location = "./sample.in";
    string output_file_location = "./test.out";
    int target_row = 1;
    int target_col = 1;
    char t_color = '4';

    int component_count = 1;
    int row_col[2];
    int row = 0;
    int col = 0;
    char init_c = '-';
    int dir[][2] = {{1,0}, {0,1}, {0,-1}, {-1,0}};

// open file
    FILE *image_file = fopen(image_file_location.c_str(), "rb");
    if (!image_file) {
        fprintf(stderr, "file not found\n");
        fclose(image_file);
        return 1;
    }

// read image size
    {
        char* line=NULL;
        size_t len = 0;
        getline(&line, &len, image_file);
        char* pch = strtok (line," ");
        int i=0;
        while (pch != NULL && i<2){
            row_col[i++] = atoi(pch);
            pch = strtok (NULL, " ");
        }
    }

// read image array
    row = row_col[0]; col = row_col[1];
    char image_ary[row+2][col+2];
    for(int i=0; i<row+2; i++){
        for(int j=0; j<col+2; j++){
            image_ary[i][j] = init_c;
        }
    }
    {
        char c;
        int i=0;
        while (c != EOF){ 
            c = fgetc(image_file);
            if(c==' '||c=='\n') continue;
            image_ary[(i/col)+1][(i%col)+1] = c;
            i++;
        } 
    }
    fclose(image_file);

// make connected array and component_count the number of connected component
    int conn_ary[row+2][col+2];
    for(int i=0; i<row+2; i++){
        for(int j=0; j<col+2; j++){
            conn_ary[i][j] = 0;
        }
    }
    for(int i=1; i<row+1; i++){
        for(int j=1; j<col+1; j++){
            if(image_ary[i][j]!=init_c && conn_ary[i][j]==0){
                queue<struct triple> q;
                q.push( triple(i, j, image_ary[i][j]));

                conn_ary[i][j] = component_count;
                while(!q.empty()){
                    struct triple now = q.front();
                    q.pop();
                    conn_ary[now.row][now.col] = component_count;
                    for(int x=0; x<4; x++){
                        int n_r = now.row+dir[x][0];
                        int n_c = now.col+dir[x][1];
                        if( image_ary[n_r][n_c] == now.c && conn_ary[n_r][n_c]==0){
                            q.push( triple(n_r, n_c, now.c));
                            
                        }
                    }
                }
                component_count++;
            }
        }
    }

// find target pixel and change the color
    queue<struct triple> q;
    int this_com = conn_ary[target_row][target_col];
    q.push( triple(target_row, target_col, image_ary[target_row][target_col]));
    while(!q.empty()){
        struct triple now = q.front();
        q.pop();
        image_ary[now.row][now.col] = t_color;
        for(int x=0; x<4; x++){
            int n_r = now.row+dir[x][0];
            int n_c = now.col+dir[x][1];
            if( conn_ary[n_r][n_c] == this_com && image_ary[n_r][n_c]!=t_color){
                q.push( triple(n_r, n_c, now.c));
            }
        }
    }

// write output
    FILE *output = fopen(output_file_location.c_str(),"wb");
    char blank = ' ';
    char nextL = '\n';
    char firstL[2];
    sprintf(firstL,"%d", row_col[0]);
    fwrite(&firstL, 1, 1, output);
    fwrite(&blank, 1, 1, output);
    sprintf(firstL,"%d", row_col[1]);
    fwrite(&firstL, 1, 1, output);
    fwrite(&nextL, 1, 1, output);
    for(int i=1; i<row+1; i++){
        for(int j=1; j<col+1; j++){
            char c = image_ary[i][j];
            fwrite(&c, 1, 1, output);
            fwrite(&blank, 1, 1, output);
        }
        fwrite(&nextL, 1, 1, output);
    }
    int digit = log10(component_count)+1;
    char lastL[digit];
    sprintf(lastL, "%d", component_count);
    fwrite(&lastL, 1, digit, output);


    return 0;
}