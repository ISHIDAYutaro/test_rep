//20120627 ISHIDA Yutaro
//e_simulator.c
//仮想陰極発振器周辺の電界分布の解析

#include<stdio.h>
#include<math.h>

int main(void)
{
	FILE *fi;
	
	fi=fopen("e_simulator_1.txt","w");
	
	double rc; //仮想陰極発振器カソード半径[m]
	double dg; //仮想陰極発振器ギャップ長[m]
	double interval; //差分法メッシュ間隔[m]
	double x_space; //左余地[m]
	double y_space; //上下余地[m]
	int x; //差分法メッシュ横軸
	int y; //差分法メッシュ縦軸
	int tri_width; //カソード部の電圧v[][]に1を入れる際の三角の高さ
	int tri_height; //カソード部の電圧v[][]に1を入れる際の三角の底辺
	int x_posi;
	int y_posi;
	int x_posi_buf;
	int y_posi_buf;
	double diff; //前回との差分
	double diff_buf; //前回との差分の一時記憶
	
	rc=10.0*pow(10,-3);
	dg=2.0*pow(10,-3);
	interval=0.1*pow(10,-3);
	x_space=18*pow(10,-3);
	y_space=10*pow(10,-3);
	x=(x_space+2*rc+dg)/interval; //左余地+カソード半径+ギャップ長
	y=(y_space+2*rc+y_space)/interval; //上余地+カソード半径+下余地
	
	double v[x+1][y+1]; //各点での電圧
	double v_buf[x+1][y+1]; //各点での電圧の一時記憶
	
	//各点での電圧v[][]を空にする
	for(x_posi=0;x_posi<=x;x_posi++){
		for(y_posi=0;y_posi<=y;y_posi++){
			v[x_posi][y_posi]=0;
		}
	}
	
	for(x_posi=300;x_posi>=0;x_posi--){
		for(y_posi=250;y_posi>=150;y_posi--){
			v[x_posi][y_posi]=1;
		}
	}
	
	//カソード部の電圧v[][]に1を入れる
	for(tri_width=rc/interval;tri_width>=0;tri_width--){
		tri_height=sqrt(pow(rc/interval,2)-pow(tri_width,2));
		
		x_posi=x_space/interval+rc/interval-tri_width;
		v[x_posi][y/2]=1;
		for(y_posi=y/2-1;y_posi>=y/2-tri_height;y_posi--){ //カソード左上
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
		for(y_posi=y/2+1;y_posi<=y/2+tri_height;y_posi++){ //カソード左下
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
		
		x_posi=x_space/interval+rc/interval+tri_width;
		v[x_posi][y/2]=1;
		for(y_posi=y/2-1;y_posi>=y/2-tri_height;y_posi--){ //カソード右上
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
		for(y_posi=y/2+1;y_posi<=y/2+tri_height;y_posi++){ //カソード右下
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
	}
	
	/*
    for(x_posi=0;x_posi<=(int)x;x_posi++){
        for(y_posi=0;y_posi<=(int)y;y_posi++){
            fprintf(fi,"%d %d %6f\n",x_posi,y_posi,v[x_posi][y_posi]);             
        }
    }
	*/
	
	//有効桁数7桁
	
	for(;;){
		diff=0;
		for(x_posi=0;x_posi<=x;x_posi++){
			for(y_posi=0;y_posi<=y;y_posi++){
				if(v[x_posi][y_posi]!=1){ //仮想発振器カソード部以外
					
					v_buf[x_posi][y_posi]=v[x_posi][y_posi];
					
					if(x_posi==0){
						if(y_posi==0){ //左上角の場合
							v[x_posi][y_posi]=(v[0][1]+v[1][0])/2;
						}
						else if(y_posi==y){ //左下角の場合
							v[x_posi][y_posi]=(v[0][y-1]+v[1][y])/2;
						}
						else{ //左端の場合
							v[x_posi][y_posi]=(v[0][y_posi-1]+v[0][y_posi+1]+v[1][y_posi])/3;
						}
					}
					else if(x_posi==x){
						/*
						if(y_posi==0){ //右上角の場合
							v[x_posi][y_posi]=(v[x][1]+v[x-1][0])/2;
						}
						else if(y_posi==y){ //右下角の場合
							v[x_posi][y_posi]=(v[x][y-1]+v[x-1][y])/2;
						}
						else{ //右端の場合
							v[x_posi][y_posi]=(v[x][y_posi-1]+v[x][y_posi+1]+v[x-1][y_posi])/3;
						}
						*/
						v[x_posi][y_posi]=0;
					}
					else if(y_posi==0){ //上端の場合
						v[x_posi][y_posi]=(v[x_posi-1][0]+v[x_posi+1][0]+v[x_posi][1])/3;
					}
					else if(y_posi==y){ //下端の場合
						v[x_posi][y_posi]=(v[x_posi-1][y]+v[x_posi+1][y]+v[x_posi][y-1])/3;
					}
					else{ //その他の場合
						v[x_posi][y_posi]=(v[x_posi-1][y_posi]+v[x_posi+1][y_posi]+v[x_posi][y_posi-1]+v[x_posi][y_posi+1])/4;
					}
					
					diff_buf=v[x_posi][y_posi]-v_buf[x_posi][y_posi];
					
					if(diff<diff_buf){
						diff=diff_buf;
						x_posi_buf=x_posi;
						y_posi_buf=y_posi;
					}
					
				}
			}
	    }
		printf("%10.10f %d %d\n",diff,x_posi_buf,y_posi_buf);
		if(diff<0.0001){
			break;
		}
	}
	
	
	for(x_posi=300;x_posi>=0;x_posi--){
		for(y_posi=250;y_posi>=150;y_posi--){
			v[x_posi][y_posi]=1;
		}
	}
	
	for(tri_width=rc/interval;tri_width>=0;tri_width--){
		tri_height=sqrt(pow(rc/interval,2)-pow(tri_width,2));
		
		x_posi=x_space/interval+rc/interval-tri_width;
		v[x_posi][y/2]=1;
		for(y_posi=y/2-1;y_posi>=y/2-tri_height;y_posi--){ //カソード左上
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
		for(y_posi=y/2+1;y_posi<=y/2+tri_height;y_posi++){ //カソード左下
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
		
		x_posi=x_space/interval+rc/interval+tri_width;
		v[x_posi][y/2]=1;
		for(y_posi=y/2-1;y_posi>=y/2-tri_height;y_posi--){ //カソード右上
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
		for(y_posi=y/2+1;y_posi<=y/2+tri_height;y_posi++){ //カソード右下
			//printf("%d %d\n",x_posi,y_posi);
			v[x_posi][y_posi]=1;
		}
	}	
	
	for(x_posi=0;x_posi<=x;x_posi++){
		for(y_posi=0;y_posi<=y;y_posi++){
			fprintf(fi,"%d %d %10.10f %d %d\n",x_posi,y_posi,v[x_posi][y_posi],x_posi,y_posi);
		}
	}
	
	
	/*
	for(x_posi=0;x_posi<=150;x_posi++){
		for(y_posi=0;y_posi<=150;y_posi++){
			fprintf(fi,"%d %d %10.10f\n",x_posi,y_posi,v[x_posi+(x-150)][y_posi+(y/2-75)]);
		}
	}
	*/
	
	fclose(fi);
	
	return 0;
}