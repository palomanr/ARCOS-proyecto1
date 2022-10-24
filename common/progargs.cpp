//
// Created by palom on 22/10/2022.
//
//
// Created by palom on 21/10/2022.
//
#include "common.hpp"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <filesystem>


namespace common {

//función para comprobar la existencia de un directorio
    namespace fs = std::filesystem;
    int existe_directorio(const fs::path& p, fs::file_status s = fs::file_status{}){
        //si el file no existe
        std::cout << p;
        if(fs::status_known(s) ? fs::exists(s) : fs::exists(p))
            return 1;
        else
            return 0;
    }

    void parametros_entrada(int argc, char *argv[]) {
        //El programa deberá comprobar que el número de parámetros es 3
        if (argc != 3){
            std::cerr<<"Wrong format:\n image in_path out_path oper\n "
                       "operation: copy, histo, mono, gauss\n";
        }
        //Si el tercer argumento no toma un valor adecuado (copy, histo, mono o gauss), se presentará un
        //mensaje de error y se terminará
        if (strcmp(argv[2],"copy")!=0 or strcmp(argv[2],"histo")!=0 or strcmp(argv[2],"mono")!=0 or strcmp(argv[2],"gauss")!=0)
        {
            std::cerr<<"Unexpected operation: "<<argv[2]<<"\n image in_path out_path oper\n operation: copy, "
                                                          "histo,mono, gauss\n";
            exit( EXIT_FAILURE );
        }

        FILE* fp = std::fopen(argv[0], "r+");

        if (existe_directorio(argv[0]) == 0 or !fp){
            std::cerr<<"Input path: "<<argv[0]<<"\nOutput path: "<<argv[1]<<"\nCannot open "
                                                                            "directory "<<argv[0]<<"\n image in_path "
                                                                                                   "out_path oper\n "
                                                                                                   "operation: copy, "
                                                                                                   "histo, mono, gauss";
            exit( EXIT_FAILURE );

        }
            //Si el directorio de salida no existe, se presentará un mensaje de error y se terminará
        else if (existe_directorio(argv[1]) == 0){
            std::cerr<<"Input path: "<<argv[0]<<"\nOutput path: "<<argv[1]<<"\nCannot open "
                                                                            "directory "<<argv[1]<<"\n image in_path "
                                                                                                   "out_path oper\n "
                                                                                                   "operation: copy, "
                                                                                                   "histo, mono, gauss";
            exit( EXIT_FAILURE );
        }

        else{
            std::cout<<"continuar";
        }
    }

}
