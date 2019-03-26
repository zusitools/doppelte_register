#include "zusi_parser/zusi_types.hpp"
#include "zusi_parser/zusi_parser.hpp"
#include "zusi_parser/utils.hpp"

#include <boost/nowide/args.hpp>
#include <boost/nowide/iostream.hpp>
#include <boost/nowide/fstream.hpp>

#include <unordered_map>
#include <utility>
#include <string_view>

using namespace std::literals::string_view_literals;

std::string_view richtungToString(bool norm) {
  return norm ? "Norm"sv : "Gegen"sv;
}

int main(int argc, char** argv) {
  boost::nowide::args a(argc, argv);

  if (argc != 2) {
    boost::nowide::cerr << "Bitte eine ST3-Datei als Parameter uebergeben\n";
    return 1;
  }

  const auto& st3 = [&]() -> std::unique_ptr<Zusi> {
    try {
      return zusixml::parseFile(argv[1]);
    } catch (const std::exception& e) {
      boost::nowide::cerr << "Fehler beim Laden der ST3-Datei: " << e.what() << "\n";
      return nullptr;
    }
  }();

  if (!st3 || !st3->Strecke) {
    return 1;
  }

  std::unordered_map<int, std::pair<int, bool>> element_by_register;

  for (const auto& el : st3->Strecke->children_StrElement) {
    if (!el) {
      continue;
    }

    for (const auto& norm : { true, false }) {
      const auto& richtungsInfo = (norm ? el->InfoNormRichtung : el->InfoGegenRichtung);
      if (!richtungsInfo || (richtungsInfo->Reg < 1000)) {
        continue;
      }

      const auto& [ it, inserted ] = element_by_register.try_emplace(richtungsInfo->Reg, std::make_pair(el->Nr, norm));
      if (!inserted && (it->second.first != el->Nr)) {
        boost::nowide::cout << "Element " << el->Nr << " " << richtungToString(norm) << ": Register " << it->first << " ist auch an Element " << it->second.first << " " << richtungToString(it->second.second) << " vorhanden\n";
      }
    }

  }
}
