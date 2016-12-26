#include "json.hpp"
#include <fstream>
#include <sstream>

using json = nlohmann::json;
using namespace std;

int main() {
    std::ifstream file_a_leer("verconsulta_Piezas.json");
    json j(file_a_leer);
    json::iterator it = j["features"].begin();

    //for (int i = 0; i < j["features"].size(); ++i) {
    ostringstream os;

    os << (*it).at("properties").at("title");
    string pillar = os.str();
    cout << pillar << endl;
    //++it;
    //}

    return 0;

}
