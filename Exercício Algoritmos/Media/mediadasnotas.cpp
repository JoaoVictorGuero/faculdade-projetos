#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <locale>

using namespace std;

const int MAX_ALUNOS = 5;

float calcularMedia(float n1, float n2, float n3) {

    return (n1 + n2 + n3) / 3.0;

}

int main() 
{

    setlocale(LC_ALL, "pt_BR.UTF-8");

    vector<string> nomes(MAX_ALUNOS);
    float notas[MAX_ALUNOS][3];
    float medias[MAX_ALUNOS];
    string situacoes[MAX_ALUNOS];

    float maiorMedia = 0.0;
    vector<int> indicesMaiorMedia;

    for (int i = 0; i < MAX_ALUNOS; i++) 
    {

        cout << "Digite o nome do aluno " 
        << i + 1 
        << ": ";
        getline(cin, nomes[i]);

        cout << "Digite as 3 notas de " 
        << nomes[i] 
        << ": ";

        for (int j = 0; j < 3; j++) 
        {

            cin >> notas[i][j];

        }

        cin.ignore();

        medias[i] = calcularMedia(notas[i][0], notas[i][1], notas[i][2]);
        situacoes[i] = (medias[i] >= 7.0) ? "Aprovado" : "Reprovado";

        if (medias[i] > maiorMedia) 
        {

            maiorMedia = medias[i];
            indicesMaiorMedia.clear();
            indicesMaiorMedia.push_back(i);

        } 
        else if (medias[i] == maiorMedia) 
        {

            indicesMaiorMedia.push_back(i);

        }

        cout << endl;

    }

    system("cls"); 

    cout << left 
    << setw(12) 
    << "ALUNO" 
    << setw(6) 
    << "N1" 
    << setw(6) 
    << "N2" 
    << setw(6)
    << "N3" 
    << setw(8) 
    << "MEDIA" 
    << "SITUACAO"
    << endl;

    for (int i = 0; i < MAX_ALUNOS; i++) 
    {

        cout << left 
        << setw(12) 
        << nomes[i]
        << setw(6) 
        << fixed 
        << setprecision(1) 
        << notas[i][0]
        << setw(6) 
        << notas[i][1]
        << setw(6) 
        << notas[i][2]
        << setw(8) 
        << medias[i]
        << situacoes[i]
        << endl;

    }

    cout << "\nAluno(s) com maior media (" 
    << fixed 
    << setprecision(1) 
    << maiorMedia 
    << "): ";

    for (int i : indicesMaiorMedia) 
    {

        cout << nomes[i] 
        << " ";

    }

    cout << endl;

    return 0;

}