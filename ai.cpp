#include "stdafx.h"
#include "ai.h"

void qp::getarray(int a[8][8]){
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++)
	a[i][j]=q[i][j];
}
int qp::setdifficulty(int d){
	if(d>0&&d<6)
	difficulty=d+2;
	else if(d>=6) difficulty=7;
	else if(d<=0) difficulty=3;
	else return -1;
	return 0;
}
int qp::get(int x, int y){
	int i = q[x - 1][y - 1];
	return i;
}
void qp::pass(){
		message = "         pass";
			flag = -flag; 
			passsum++;
			struct board temp(q, available, flag);
			boardlink.push_back(temp);
}
qp::point qp::getnextstep(board b, int count){
	count += 1;
	int locatescore, sumscore, nextscore, score;
	point p(0, 0);
	p.score = -1000;
	p.flag = -100;
	set<point>::iterator it;
	for (it = b.available.begin(); it != b.available.end(); it++){
		board nb = b;
		if (getnextboard(nb, *it) == -1){
			//	if(count==2){
			//	cout<<it->x<<" "<<it->y<<" "<<it->score<<endl;	}
			//point temp = *it;
			//temp.score = -10000;
			//b.available.erase(*it);
			//b.available.insert(temp);

			continue;
		}
		else{
			if (nb.flag == 1)
				sumscore = nb.osum - nb.xsum;
			else if (nb.flag == -1)
				sumscore = nb.xsum - nb.osum;
			if (count<difficulty&& nb.step <= xm*ym - 4)
				nextscore = getnextstep(nb, count).score;
			else nextscore = 0;
			locatescore = scorearray[it->x][it->y];
			score = locatescore + sumscore - nextscore;
			point temp = *it;
			temp.score = score;
		//	if (count == 2)
				//cout << it->x << " " << it->y << " " << temp.score << endl;
			board nbb = b;
			if (getnextboard(nbb, *it) == -1){
				continue;
			}
			else{
				if (p.flag<0) p = temp;
				if (temp.score>p.score) p = temp;
			}
			//b.available.erase(*it);
			//	b.available.insert(temp);
		}
	}
	/*	point p=*(b.available.begin());
	for(it=b.available.begin();it!=b.available.end();it++){

	if(it->flag==0){
	if(count==2){
	cout<<it->x<<" "<<it->y<<" "<<it->score<<endl;}
	board nb=b;
	if(getnextboard(nb,*it)==-1){
	cout<<"louwang"<<endl;
	continue;
	}
	if(p.flag==-1)p=*it;
	if(it->score>p.score) p=*it;
	}
	}*/
	return p;
}
int qp::input(int x, int y){
	if (xsum == 0){
		message = "      white win";
		return 100;
	}
	if (osum == 0){
		message = "      black win";
		return 100;
	}
	if (xsum + osum == xm*ym) {
		message = "      the game is over";
		return -100;
	}
	if (x>xm || x<1 || y>ym || y<1){
		message = "      input err";
		return -1;
	}
	if (q[x - 1][y - 1] != 0){
		message = "      overlaped";
		return -1;
	}
	struct board temp(q, available,flag);
	point p(x - 1, y - 1);
	//cout << "flag:" << temp.flag << "step" << temp.step << endl;
	if (getnextboard(temp, p) != -1){
		flag = -flag;
		step++;
		memcpy(q, temp.q, sizeof(temp.q));
		available = temp.available;
		boardlink.push_back(temp);
		xsum = temp.xsum;
		osum = temp.osum;
	}
	else{
		message = "      can't go there";
		return-1;
	}
	return 1;
}
int qp::getnextboard(board &b, point p)
{
	int f = b.flag;
	if (p.x<0 || p.y<0 || p.x >= xm || p.y >= ym){
		return -1;
	}
	bool can = false;
	int xx, yy, op = 0;
	int x = p.x, y = p.y;
	for (xx = x + 1; xx<xm; xx++){

		if (b.q[xx][y] == 0) break;
		if (b.q[xx][y] == -f) { op += 1; continue; }
		if (b.q[xx][y] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x + i][y] = f;
				i++; op--;
			}
			can = true;
			break;
		}
	}
	op = 0;
	for (xx = x - 1; xx >= 0; xx--){
		if (b.q[xx][y] == 0) break;
		if (b.q[xx][y] == -f) { op += 1; continue; }
		if (b.q[xx][y] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x - i][y] = f;
				i++; op--;
			}
			can = true;
			break;
		}
	}
	op = 0;
	for (yy = y + 1; yy<ym; yy++){
		if (b.q[x][yy] == 0) break;
		if (b.q[x][yy] == -f) { op += 1; continue; }
		if (b.q[x][yy] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x][y + i] = f;
				i++; op--;
			}
			can = true;
			break;
		}
	}
	op = 0;
	for (yy = y - 1; yy >= 0; yy--){
		if (b.q[x][yy] == 0) break;
		if (b.q[x][yy] == -f) { op += 1; continue; }
		if (b.q[x][yy] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x][y - i] = f;
				i++; op--;
			}
			can = true;
			break;
		}
	}
	op = 0;
	for (xx = x + 1, yy = y + 1; xx<xm&&yy<ym; xx++, yy++){
		if (b.q[xx][yy] == 0) break;
		if (b.q[xx][yy] == -f) { op += 1; continue; }
		if (b.q[xx][yy] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x + i][y + i] = f;
				i++; op--;
			}
			can = true;
			break;
		}
	}
	op = 0;
	for (xx = x - 1, yy = y - 1; xx >= 0 && yy >= 0; xx--, yy--){
		if (b.q[xx][yy] == 0) break;
		if (b.q[xx][yy] == -f) { op += 1; continue; }
		if (b.q[xx][yy] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x - i][y - i] = f;
				i++; op--;
			}
			can = true;
			break;
		}
	}
	op = 0;
	for (xx = x + 1, yy = y - 1; xx<xm&&yy >= 0; xx++, yy--){
		if (b.q[xx][yy] == 0) break;
		if (b.q[xx][yy] == -f) { op += 1; continue; }
		if (b.q[xx][yy] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x + i][y - i] = f;
				i++; op--;
			}
			can = true;
			break;
		}
	}
	op = 0;
	for (xx = x - 1, yy = y + 1; xx >= 0 && yy<ym; xx--, yy++){
		if (b.q[xx][yy] == 0) break;
		if (b.q[xx][yy] == -f) { op += 1; continue; }
		if (b.q[xx][yy] == f)
		if (op == 0) break;
		else{
			int i = 1;
			while (op>0){
				b.q[x - i][y + i] = f;
				i++; op--;
			}
			can = true;
			break;
		}

	}
	op = 0;
	if (can == false){
		return -1;
	}
	else{
		b.q[x][y] = f;
		b.flag = -b.flag;
		b.step++;
		if (q[x + 1][y] == 0 && x + 1<xm)
			b.available.insert(point(x + 1, y));
		if (q[x - 1][y] == 0 && x - 1 >= 0)
			b.available.insert(point(x - 1, y));
		if (q[x][y + 1] == 0 && y + 1<ym)
			b.available.insert(point(x, y + 1));
		if (q[x][y - 1] == 0 && y - 1 >= 0)
			b.available.insert(point(x, y - 1));
		if (q[x + 1][y + 1] == 0 && x + 1<xm&&y + 1<ym)
			b.available.insert(point(x + 1, y + 1));
		if (q[x - 1][y + 1] == 0 && x - 1 >= 0 && y + 1<ym)
			b.available.insert(point(x - 1, y + 1));
		if (q[x + 1][y - 1] == 0 && x + 1<xm&&y - 1 >= 0)
			b.available.insert(point(x + 1, y - 1));
		if (q[x - 1][y - 1] == 0 && x - 1 >= 0 && y - 1 >= 0)
			b.available.insert(point(x - 1, y - 1));
		b.available.erase(point(x, y));
		struct board temp(b.q, b.available,b.flag);
		b.xsum = temp.xsum;
		b.osum = temp.osum;

		return 1;
	}
}

void qp::testprint(){
	cout << "flag" << flag<<endl;
	for (int i = 0; i<xm; i++){
		for (int j = 0; j<ym; j++)
			printf("%d", q[i][j]);
		cout << endl;
	} printf("xsum:%d,osum:%d\n", xsum, osum);
	set<point>::iterator it;
	cout << available.size(); for (it = available.begin(); it != available.end(); it++)
		cout << it->x + 1 << "," << it->y + 1 << endl;
}
int qp::back(int b){
	for (int i = 0; i<b; i++){
		if (step == 0){
		message = "no precious step";
		return -1;
	}
		boardlink.pop_back();
	struct board temp(boardlink.back().q, boardlink.back().available,boardlink.back().flag);
	step = temp.step;
	memcpy(q, temp.q, sizeof(temp.q));
	available = temp.available;
	xsum = temp.xsum;
	osum = temp.osum;
	flag = temp.flag;
	message = "       back";
	}
	return 0;
}
int qp::pvc(int x, int y){
	message = "      thinking...";
	if (shengfu() != 0){
		print(); 
		return -1; 
	}
	if (input(x, y) == -1) {
		print();
		return -1;
	}
	else{}
	
	return 0;
}
int qp::cvc(){
		message="";
		point p = getnextstep(boardlink.back(), 1);
		cout << "p:" << p.x + 1 << ',' << p.y + 1 << endl;
		if (p.flag < 0) {
			pass();
		}
		else if (input(p.x + 1, p.y + 1) != 1) {
			cout << message << "input errrrrrrr" << endl;
			return -1;
		}
		else if (p.flag>=0){
			passsum=0;
		}
		if (shengfu() != 0){ print();return 0; }
		print();
		return 0;
}
int qp::shengfu(){
	if ((xsum + osum == xm*ym)||(passsum==2)){
		if (xsum > osum){ message = "     black win"; return 1; }
		else if (xsum < osum){ message = "     white win";return 2; }
		else if (xsum == osum){ message = "      pingju"; return 3; }

	}
	else {return 0;}
	return 0;
}
void qp::print(){
	int t = 0;
	//testprint();
	cout << ' ';
	for (int i = 0; i<ym; i++)
		printf("%2d", i + 1);
	cout << endl;
	for (int i = 0; i<xm; i++){
		printf("%2d", i + 1);
		for (int j = 0; j<ym; j++){
			t = q[i][j];
			switch (t){
			case 0:cout << ' ';
				break;
			case 1:cout << 'x'; break;
			case -1:cout << 'o'; break;
			}
			if (j != ym-1) cout << '-';

		}
		if (i == xm / 4) printf(message);
		if (i == xm / 2)
			printf("     xsum:%d;osum:%d", xsum, osum);
		cout << endl << "  ";
		if (i != xm-1)
		for (int u = 0; u<xm; u++)
			cout << "| ";
		cout << endl;
	}
	message = "";
	char *turns;
	(flag==1)?turns="blank":turns="white";
	printf("step:%d    turn:%s    difficulty:%d\nplease input the position:",step,turns,difficulty-2);
}

/*
int main() {
int x, y, d;
qp test;
printf("input difficulty(1-5):");
scanf_s("%d", &d);
test.setdifficulty(d);
test.print();
//	 while(true){test.cvc();}
while (true) {
scanf_s("%d,%d", &x, &y);
if (x == 100) {
test.pass();
test.print();
test.cvc();
}
else if (x > 0 && y > 0) {
test.pvc(int(x), int(y));
}
else {
test.back();
test.print();
}

}
return 0;
}

*/