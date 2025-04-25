#include <iostream>
#include <math.h>
#include <ctime>
#include <fstream>
#include <string>
#include <limits>


#define MAX_QUAN 1000
#define DATA_FILE "market_data.dat"
using namespace std;

// Armazena informações de erro para o tratamento de entradas.
struct errorEntity{
    string cinTitle;
    string message;
    bool errorCondition;

    errorEntity(string title = "Insira novamente", string msg = "Erro desconhecido", bool condition = false) {
        cinTitle = title;
        message = msg;
        errorCondition = condition;
    }
};


/*
* Título: Limpa o buffer de entrada.
*
* Descrição: Essa função tem como objetivo limpar o buffer
* de entrada do `cin`, garantindo que entradas inválidas ou
* caracteres residuais não interfiram nas próximas leituras.
*/
void clearCinBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/*
 * Título: Verifica e trata erros de entrada no fluxo padrão (cin).
 * 
 * Descrição: Esta função monitora o fluxo de entrada padrão (cin) para detectar erros,
 * como entradas inválidas ou condições específicas de erro definidas por 
 * `errorEntity`.
 * 
 * @param value: Referência para a variável que armazenará o valor de entrada válido.
 * @param errorEntity: Estrutura contendo a condição de erro (`errorCondition`), 
 * a mensagem de erro (`message`) e o título do campo de entrada (`cinTitle`).
 */
void checkForCinError(float &value, errorEntity errorEntity) {
    while(cin.fail() || errorEntity.errorCondition) {
        clearCinBuffer();
        cout << "VALOR INADEQUADO: "<<errorEntity.message<< endl;
        cout <<errorEntity.cinTitle<<": ";
        cin >> value;
    }
    clearCinBuffer();
}

// Estrutura que lida com datas.
struct date {
    unsigned int day, month, year;

    /*
    * Título: Obtêm uma data.
    *
    * Descrição: Essa função possui a finalidade de gerar 
    * uma data com base em um parâmetro.
    * 
    * @param addDays: dias a serem somados na data.
    */
    void getDate(int addDays = 0) {
        time_t now = time(0);
        now += addDays * 86400;
        tm *localTime = localtime(&now);
        day = localTime->tm_mday;
        month = localTime->tm_mon + 1;
        year = localTime->tm_year + 1900;
    }

    // Exibe a data formatada no console.
    void showFormattedDate() {
        cout << day << "/" << month << "/" << year << endl;
    }
};

// Estrutura que lida com o produto.
struct product {
    string name;
    float quantity;
    float price;
    date editDate;

    product() {
        editDate.getDate();
    }

    /*
    * Título: Altera a quantidade atual.
    *
    * Descrição: Essa função possui a finalidade de alterar,
    * quando possível, a quantidade do produto com base no
    * valor do parâmetro.
    * 
    * @param newQuantity valor a ser adicionado/removido.
    * 
    * @return: 
    *   - true: Alteração feita com êxito.
    *   - false: Não é possível alterar (número negativo).
    */
    bool changeProductQuantity(float newQuantity) {
        if (quantity + newQuantity >= 0) {
            quantity += newQuantity;
            if (newQuantity != 0) {
                editDate.getDate();
            }
            return true;
        }
        return false;
    }

    /*
    * Título: Edita os atributos do produto.
    *
    * Descrição: Essa função possui a finalidade de lidar
    * com os métodos e processos para alterar as características
    * do priduto
    * 
    * @param newQuantity: valor a ser adicionado/removido.
    * @param newValue: novo preço.
    */
    void updateProduct(float newQuantity, float newValue) {
        changeProductQuantity(newQuantity);
        price = newValue;
    }

    // Mostra no console as informações do produto.
    void showProductInfo() {
        cout << "Nome: " << name << endl;
        cout << "Quantidade: " << quantity << " unidades" << endl;
        cout << "Valor: R$" << price << " por unidade" << endl;
        cout << "Alterado em: ";
        editDate.showFormattedDate();
    }
};

// Estrutura que lida com o estoque.
struct stock {
    product allProducts[MAX_QUAN];
    unsigned int currentQuantity;

    // Construtor padrão.
    stock() {
        currentQuantity = 0;
    }

    /*
    * Título: Encontra determinado produto no estoque.
    *
    * Descrição: Essa função possui a finalidade de encontrar, 
    * caso exista, o índice de acesso de um produto com 
    * determinado nome.
    * 
    * @param name: nome do produto.
    * 
    * @return -1 (Não existe), != -1 (encontrado)
    */
    int findProduct(string name) {
        for (unsigned int i = 0; i < currentQuantity; i++) {
            if (allProducts[i].name == name) {
                return i;
            }
        }
        return -1;
    }

    /*
    * Título: Cria um novo produto.
    *
    * Descrição: Essa função possui a finalidade de criar, 
    * caso possível, um novo produto utilizando como base
    * os parâmetros.
    * 
    * @param name: nome.
    * @param quantity: quantidade.
    * @param value: preço.
    * 
    * @return
    *   - true: Criação feita com êxito.
    *   - false: Não é possível criar (limite atingido).
    */
    bool newProduct(string name, float quantity, float value) {
        if (currentQuantity >= MAX_QUAN) return false;
        allProducts[currentQuantity].name = name;
        allProducts[currentQuantity].quantity = quantity;
        allProducts[currentQuantity].price = value;
        currentQuantity++;
        return true;
    }

    /*
    * Título: Adiciona/Edita um produto no estoque.
    *
    * Descrição: Essa função possui a finalidade de adicionar
    * ou editar determinado produto no estoque.
    * 
    * @param name: nome.
    * @param quantity: quantidade.
    * @param value: preço.
    * 
    * @return
    *   - true: Produto adicionado/editado com êxito.
    *   - false: Não é possível adicionar (limite atingido).
    */
    bool addProduct(string name, float quantity, float value) {
        bool status = true;
        int productIndex = findProduct(name);
        if (productIndex != -1)
            allProducts[productIndex].updateProduct(quantity, value);
        else {
            status = newProduct(name, quantity, value);
            if (!status) {
                cout << "Limite de produtos inseridos atingido (" << MAX_QUAN << ")." << endl;
            }
        }
        return status;
    }

    /*
    * Título: Remove o cadastro de um produto no estoque.
    *
    * Descrição: Essa função possui a finalidade de remover
    * um produto completamente, ou seja, todos os seus dados.
    * 
    * @param name: nome.

    * @return
    *   - true: Produto removido com êxito.
    *   - false: Não foi possível remover.
    */
    bool removeProduct(string name) {
        if (currentQuantity > 0) {
            int removeProductIndex = findProduct(name);
            if (removeProductIndex != -1) {
                for (unsigned int i = removeProductIndex; i < currentQuantity - 1; i++) {
                    allProducts[i] = allProducts[i + 1];
                }
                currentQuantity--;
                return true;
            }
            cout << "O produto " << name << " inexiste no estoque." << endl << endl;
        }
        else
        cout << "ESTOQUE VAZIO NO MOMENTO!" << endl << endl;

        return false;
    }

    // Mostra todas as informações dos produtos no estoque.
    void showStock() {
        if (currentQuantity > 0) {
            for (unsigned int i = 0; i < currentQuantity; i++) {
                cout << string(50, '*') << endl << endl;
                cout << "Produto " << (i + 1) << endl << endl;
                allProducts[i].showProductInfo();
                cout << endl;
            }
        }
        else {
            cout << string(50, '*') << endl;
            cout << "ESTOQUE VAZIO NO MOMENTO!" << endl << endl;
        }
    }

    /*
    * Título: Salva os dados do estoque em um arquivo.
    *
    * Descrição: Essa função tem como objetivo salvar todas as informações
    * do estoque em um arquivo para persistência de dados.
    * 
    * @param filename: Nome do arquivo onde os dados serão salvos.
    *
    * @return
    *   - true: Dados salvos com sucesso.
    *   - false: Falha ao salvar os dados.
    */
    void saveToFile() {
        ofstream outFile(DATA_FILE, ios::binary);
        if (!outFile) {
            cerr << "Erro ao criar arquivo de dados! Execute novamente o software." << endl;
            return;
        }

        outFile.write(reinterpret_cast<const char*>(&currentQuantity), sizeof(currentQuantity));

        for (unsigned int i = 0; i < currentQuantity; i++) {
            const product& p = allProducts[i];

            size_t nameSize = p.name.size();
            outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
            outFile.write(p.name.c_str(), nameSize);

            outFile.write(reinterpret_cast<const char*>(&p.quantity), sizeof(p.quantity));
            outFile.write(reinterpret_cast<const char*>(&p.price), sizeof(p.price));
            outFile.write(reinterpret_cast<const char*>(&p.editDate.day), sizeof(p.editDate.day));
            outFile.write(reinterpret_cast<const char*>(&p.editDate.month), sizeof(p.editDate.month));
            outFile.write(reinterpret_cast<const char*>(&p.editDate.year), sizeof(p.editDate.year));
        }

        outFile.close();
        cout << "Dados do estoque salvos com sucesso!" << endl;
    }
    
    /*
    * Título: Carrega os dados do estoque a partir de um arquivo.
    *
    * Descrição: Essa função tem como objetivo carregar as informações
    * do estoque previamente salvas em um arquivo.
    * 
    * @param filename: Nome do arquivo de onde os dados serão carregados.
    *
    * @return
    *   - true: Dados carregados com sucesso.
    *   - false: Falha ao carregar os dados.
    */
    bool loadFromFile() {
        ifstream inFile(DATA_FILE, ios::binary);
        if (!inFile) {
            return false;
        }

        currentQuantity = 0;

        unsigned int count;
        inFile.read(reinterpret_cast<char*>(&count), sizeof(count));

        if (count > MAX_QUAN) {
            count = MAX_QUAN;
        }

        for (unsigned int i = 0; i < count; i++) {
            product p;

            size_t nameSize;
            inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
            char* nameBuffer = new char[nameSize + 1];
            inFile.read(nameBuffer, nameSize);
            nameBuffer[nameSize] = '\0';
            p.name = nameBuffer;                     
            delete[] nameBuffer;

            inFile.read(reinterpret_cast<char*>(&p.quantity), sizeof(p.quantity));
            inFile.read(reinterpret_cast<char*>(&p.price), sizeof(p.price));
            inFile.read(reinterpret_cast<char*>(&p.editDate.day), sizeof(p.editDate.day));
            inFile.read(reinterpret_cast<char*>(&p.editDate.month), sizeof(p.editDate.month));
            inFile.read(reinterpret_cast<char*>(&p.editDate.year), sizeof(p.editDate.year));

            if (i < MAX_QUAN) {
                allProducts[i] = p;
                currentQuantity++;
            }
        }

        inFile.close();
        return true;
    }
};

// Estrutura que lida com o usuário.
struct user {
    product userProducts[MAX_QUAN];
    unsigned int currentQuantity;

    // Construtor padrão.
    user() {
        currentQuantity = 0;
    };

    /*
    * Título: Inicia o processo de compra.
    *
    * Descrição: Essa função possui a finalidade de adicionar 
    * determinado produto a lista de compras de usuário, assim
    * como realizar alterações no estoque para permitir essa ação.
    * 
    * @param storage: estoque.
    * @param name: nome.
    * @param quantity: quantidade.
    *
    * @return
    *   - true: Compra feita com êxito.
    *   - false: Não é possível comprar.
    */
    bool buyProduct(stock &storage, string name, float quantity) {
        int storageIndex = storage.findProduct(name);
        if (storageIndex != -1) {
            if (currentQuantity < MAX_QUAN) {
                bool canBuy = storage.allProducts[storageIndex].changeProductQuantity(-quantity);
                if (canBuy) {
                    userProducts[currentQuantity].name = storage.allProducts[storageIndex].name;
                    userProducts[currentQuantity].quantity = quantity;
                    userProducts[currentQuantity].price = storage.allProducts[storageIndex].price;
                    currentQuantity++;
                    return true;
                }
                else cout << "Falta quantidade suficiente no estoque." << endl << endl;
            }
            else cout << "Limite de compras atingido (" << MAX_QUAN << ")," << endl << endl;
        }
        else cout << "Compra do produto "<< name << " cancelada! Ele inexiste no estoque." << endl << endl;
        return false;
    }

    // Calcula o preço total das compras.
    float getTotal() {
        float shopPrice = 0;
        for (unsigned int i = 0; i < currentQuantity; i++) {
            shopPrice += userProducts[i].price * userProducts[i].quantity;
        }
        return shopPrice;
    }

    // Calcula juros compostos do valor total e retorna em parcelas.
    float paymentCalc(int months, float tax = 1) {
        float total = getTotal() * (pow(tax, months));
        return total / months;
    }

    // Mostra um relatório exibindo as compras do usuário.
    void showShopsRelatory() {
        if (currentQuantity > 0) {
            float shopPrice = 0;
            for (unsigned int i = 0; i < currentQuantity; i++) {
                float localPrice = userProducts[i].price * userProducts[i].quantity;
                cout << string(50, '*') << endl;
                cout << "Compra " << (i + 1) << endl << endl;
                userProducts[i].showProductInfo();
                cout << endl;
                cout << "Custo total: R$ " << localPrice << endl << endl;
                shopPrice += localPrice;
            }
            cout << string(50, '*') << endl;
            cout << "VALOR TOTAL DA COMPRA: R$ " << shopPrice << endl;
        }
        else {
            cout << string(50, '*') << endl;
            cout << "SEM COMPRAS AINDA!" << endl;
            cout << string(50, '*') << endl;
        }
    }
};

// Estrutura que lida com o mercado.
struct market {
    stock storage;
    user client;

    // Construtor padrão.
    market() {
        if (storage.loadFromFile()) {
            cout << "Estoque carregado com sucesso do arquivo." << endl;
        } else {
            cout << "Nenhum dado de estoque encontrado. Iniciando com estoque vazio." << endl;
        }
    }
    
    // Destrutor padrão.
    ~market() {
        storage.saveToFile();
    }
};

market myMarket; //GLOBAL

// Função que exibe o menu principal.
char masterMenu() {
    char option;
    cout << "BEM VINDO AO SISTEMA DE MERCADO!" << endl;
    cout << "Este software possibilita gerenciar produtos e compras." << endl << endl;
    cout << string(50, '-') << endl << endl;
    cout << "Produtos: " << endl;
    cout << "A - Cadastrar / Editar" << endl;
    cout << "B - Deletar" << endl;
    cout << "C - Comprar" << endl << endl;
    cout << string(50, '-') << endl << endl;
    cout << "Outros: " << endl;
    cout << "D - Exibir produtos" << endl;
    cout << "E - Salvar manualmente" << endl;
    cout << "F - Sair" << endl << endl;
    cout << string(50, '-') << endl << endl;
    cout << "Insira um caractere: ";
    cin >> option;
    clearCinBuffer();
    cout << endl;
    return option;
}

// Função que lida com o menu de cadastro.
void registerProductMenu() {
    string name;
    float quantity, price;
    cout << string(50, '-') << endl << endl;
    cout << "Cadastrar/editar produto..." << endl << endl;
    cout << "C - Insira no campo 'Nome' para cancelar o cadastro." << endl<<endl;
    cout << "Nome: ";
    getline(cin, name);
    bool productExists = myMarket.storage.findProduct(name) != -1;
    if(name != "C"){
        cout << "Quantidade: ";
        cin >> quantity;
        if(productExists)
            checkForCinError(quantity, errorEntity("Quantidade", "A quantidade deve ser um algarismo."));
        else
            checkForCinError(quantity, errorEntity("Quantidade", "A quantidade deve ser um algarismo e maior ou igual a 0.", quantity < 0));
        cout << "Valor (R$): ";
        cin >> price;
        checkForCinError(price, errorEntity("Valor", "O valor deve ser um algarismo e maior que 0.", price <= 0));
        cout << endl;
        if (myMarket.storage.addProduct(name, quantity, price)) {
            cout << "O produto " << name << " foi adicionado/editado com sucesso!" << endl << endl;
        }
    }
    else cout<<"O cadastro foi cancelado com sucesso!"<<endl<<endl;
    cout << string(50, '-') << endl << endl;
}

// Função que lida com o menu de remoção.
void removeProductMenu() {
    string productName;
    cout << string(50, '-') << endl << endl;
    cout << "Deletar produto..." << endl << endl;
    cout << "C - Insira no campo 'Nome' para cancelar o ato de remover." << endl<<endl;
    cout << "Nome: ";
    getline(cin, productName);
    cout << endl;
    if(productName != "C"){
        if (myMarket.storage.removeProduct(productName)) {
            cout << "O produto " << productName << " foi removido com sucesso!" << endl << endl;
        }
    }
    else cout<<"Deletar foi cancelado com sucesso!"<<endl<<endl;
    cout << string(50, '-') << endl << endl;
}

// Função que lida com o menu de compra.
void buyProductMenu() {
    string name;
    float quantity;
    char option;
    cout << string(50, '-') << endl << endl;
    cout << "Comprar produtos..." << endl << endl;
    do {
        cout << "* Nova compra" << endl;
        cout << "Nome: ";
        getline(cin, name);
        cout << "Quantidade: ";
        cin >> quantity;
        checkForCinError(quantity, errorEntity("Quantidade", "A quantidade deve ser um algarismo e maior ou igual a 0.", quantity < 0));
        cout << endl;
        bool status = myMarket.client.buyProduct(myMarket.storage, name, quantity);
        if(status){
            cout << "O que deseja fazer agora?" << endl << endl;
            cout << "1 - Continuar comprando" << endl;
            cout << "2 - Finalizar compra" << endl << endl;
            cout << "Insira o algarismo: ";
            cin >> option;
            clearCinBuffer();
            cout << endl;
        }
    } while (option != '2');
}

// Função que lida com o menu de parcelas sem juros.
void paymentPortionsMenu(int months, float tax) {
    date currentDate;
    float portionValue = myMarket.client.paymentCalc(months, tax);
    cout << string(50, '-') << endl << endl;
    cout << months << " vezes sem juros..." << endl;
    for (int i = 0; i < months; i++) {
        currentDate.getDate((i + 1) * 30);
        cout << "A primeira parcela (" << i + 1 << ") deve ser paga no valor de R$ " << portionValue << ", vence no dia ";
        currentDate.showFormattedDate();
    }
}

// Função que lida com o menu de parcelas com juros.
void paymentProductsMenu() {
    char option;
    cout << string(50, '-') << endl << endl;
    cout << "Pagamento..." << endl << endl;
    cout << "Como vai ser efetuado o pagamento?" << endl << endl;
    cout << "1 - A vista (5% de desconto)" << endl;
    cout << "2 - 3 vezes sem juros" << endl;
    cout << "3 - 12 vezes com 10% de juros" << endl << endl;
    cout << "Insira o algarismo: ";
    cin >> option;
    while (option != '1' && option != '2' && option != '3') {
        cout << "Insira um algarismo correto (1, 2, 3)!" << endl;
        cout << "Insira o algarismo: ";
        cin >> option;
    }
    clearCinBuffer();
    switch (option) {
        case '1':
            cout << string(50, '-') << endl << endl;
            cout << "A vista..." << endl;
            cout << "O pagamento a vista da compra deve ser efetuado no valor de R$ " << myMarket.client.getTotal() * 0.95f << " (5% de desconto)." << endl << endl;
            break;
        case '2':
            paymentPortionsMenu(3, 1);
            break;
        case '3':
            paymentPortionsMenu(12, 1.10f);
            break;
    }
}

int main() {
    char mainOption;
    do {
        mainOption = masterMenu();
        switch (mainOption) {
            case 'A':
                registerProductMenu();
                break;
            case 'B':
                removeProductMenu();
                break;
            case 'C':
                buyProductMenu();
                myMarket.client.showShopsRelatory();
                paymentProductsMenu();
                myMarket.client.currentQuantity = 0;
                break;
            case 'D':
                myMarket.storage.showStock();
                break;
            case 'E':
                myMarket.storage.saveToFile();
                break;
            case 'F':
                cout << string(50, '-') << endl;
                cout << "Finalizando o software..." << endl << endl;
                break;
            default:
                cout << string(50, '-') << endl << endl;
                cout << "Insira um caractere correto (A, B, C, D, E ou F)!" << endl << endl;
                break;
        }
        cout << string(50, '*') << endl;
    } while (mainOption != 'F');
    return 0;
}
