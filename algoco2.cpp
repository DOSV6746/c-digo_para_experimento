#include<iostream>
#include<stack>
#include<vector>
#include<algorithm>
#include<climits>
#include<fstream>
#include<string>

using namespace std;

int cs[26][26];
int ci[26];
int cd[26];
int ct[26][26];

void cargar() {
	ifstream archivo;
	string contenido;
	archivo.open("cost_replace.txt",ios::in);
	if(archivo.fail()) {
		cout<<"No se pudo abrir el archivo"<<endl;
		exit(1);
	}
	int i = 0;
	int j = 0;
	while(!archivo.eof()) {
		string s;
		archivo>>s;
		if(s[0] == 0) break;
		cs[i][j] = stoi(s);
		if(j >= 26) {
			j = 0;
			++i;
		} else ++j;
	}
	archivo.close();
	archivo.open("cost_insert.txt",ios::in);
	if(archivo.fail()) {
		cout<<"No se pudo abrir el archivo"<<endl;
		exit(1);
	}
	i = 0;
	while(!archivo.eof()) {
		string s;
		archivo>>s;
		if(s[0] == 0) break;
		ci[i] = stoi(s);
		++i;
	}
	archivo.close();
	archivo.open("cost_delete.txt",ios::in);
	if(archivo.fail()) {
		cout<<"No se pudo abrir el archivo"<<endl;
		exit(1);
	}
	i = 0;
	while(!archivo.eof()) {
		string s;
		archivo>>s;
		if(s[0] == 0) break;
		cd[i] = stoi(s);
		++i;
	}
	archivo.close();
	archivo.open("cost_transpose.txt",ios::in);
	if(archivo.fail()) {
		cout<<"No se pudo abrir el archivo"<<endl;
		exit(1);
	}
	i = 0;
	j = 0;
	while(!archivo.eof()) {
		string s;
		archivo>>s;
		if(s[0] == 0) break;
		ct[i][j] = stoi(s);
		if(j >= 26) {
			j = 0;
			++i;
		} else ++j;
	}
	archivo.close();
	return;
}

int costo_sub(char a, char b) {
	int costo;
	costo = cs[a - 97][b - 97];
	return costo;
}

int costo_ins(char b) {
	int costo;
	costo = ci[b - 97];
	return costo;
}

int costo_del(char a) {
	int costo;
	costo = cd[a - 97];
	return costo;
}

int costo_trans(char a, char b) {
	int costo;
	costo = ct[a - 97][b - 97];
	return costo;
}

int distanciaMinimaFuerzaBrutaAux(const string &s1, const string &s2, int i, int j) {
    if(i == 0) {
    	int suma = 0;
    	for(int u = 0; u < j; ++u) {
    		suma += costo_ins(s2[u]);
    	}
    	return suma;
    }
    if(j == 0) {
    	int suma = 0;
    	for(int u = 0; u < i; ++u) {
    		suma += costo_del(s1[u]);
    	}
    	return suma;
    }
    if (s1[i - 1] == s2[j - 1]) {
        return distanciaMinimaFuerzaBrutaAux(s1, s2, i - 1, j - 1);
    }

    int insertOp = distanciaMinimaFuerzaBrutaAux(s1, s2, i, j - 1) + costo_ins(s2[j - 1]);
    int deleteOp = distanciaMinimaFuerzaBrutaAux(s1, s2, i - 1, j) + costo_del(s1[i - 1]);
    int replaceOp = distanciaMinimaFuerzaBrutaAux(s1, s2, i - 1, j - 1) + costo_sub(s1[i - 1],s2[j - 1]);

    int transposeOp = INT_MAX;
    if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
        transposeOp = distanciaMinimaFuerzaBrutaAux(s1, s2, i - 2, j - 2) + costo_trans(s1[i - 1],s1[i - 2]);
    }

    return std::min({insertOp, deleteOp, replaceOp, transposeOp});
}

int distanciaMinimaFuerzaBruta(const string &s1, const string &s2) {
	return distanciaMinimaFuerzaBrutaAux(s1,s2,s1.size(),s2.size());
}

int distanciaMinimaRecursivaAux(const std::string &s1, const std::string &s2, int i, int j, std::vector<std::vector<int>> &memo) {
    if (i == 0) {
        int suma = 0;
        for (int u = 0; u < j; ++u) {
            suma += costo_ins(s2[u]);
        }
        return memo[i][j] = suma;
    }
    if (j == 0) {
        int suma = 0;
        for (int u = 0; u < i; ++u) {
            suma += costo_del(s1[u]);
        }
        return memo[i][j] = suma;
    }

    if (memo[i][j] != -1) return memo[i][j];

    if (s1[i - 1] == s2[j - 1]) {
        return memo[i][j] = distanciaMinimaRecursivaAux(s1, s2, i - 1, j - 1, memo);
    }

    int insertOp = distanciaMinimaRecursivaAux(s1, s2, i, j - 1, memo) + costo_ins(s2[j - 1]);
    int deleteOp = distanciaMinimaRecursivaAux(s1, s2, i - 1, j, memo) + costo_del(s1[i - 1]);
    int replaceOp = distanciaMinimaRecursivaAux(s1, s2, i - 1, j - 1, memo) + costo_sub(s1[i - 1], s2[j - 1]);

    int transposeOp = INT_MAX;
    if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
        transposeOp = distanciaMinimaRecursivaAux(s1, s2, i - 2, j - 2, memo) + costo_trans(s1[i - 1], s1[i - 2]);
    }

    return memo[i][j] = std::min({insertOp, deleteOp, replaceOp, transposeOp});
}

int distanciaMinimaRecursiva(const std::string &s1, const std::string &s2) {
    int m = s1.size();
    int n = s2.size();
    std::vector<std::vector<int>> memo(m + 1, std::vector<int>(n + 1, -1));
    return distanciaMinimaRecursivaAux(s1, s2, m, n, memo);
}


int main() {
	string s1,s2;

	cin>>s1;
	cin>>s2;
	cargar();
	cout<<distanciaMinimaFuerzaBruta(s1,s2)<<endl;

	return 0;
}

