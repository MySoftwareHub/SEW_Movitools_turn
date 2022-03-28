/*=============================================
              Fichier source IPOS+

Auteur      : Aubert Guillaume
              Drenth David
              Henot Florian

Date        : 20/10/2017
Description : Gestion de la maquette SEW
Version     : V.6
===============================================*/

/*=============================================
Biblioth�ques
===============================================*/

#include <constb.h> //Fonctions-syst�mes d�finies
#include <iosb_f.h> //D�finitions des E/S

/*=============================================
Variables internes
===============================================*/

#define position0 0                    //Position � 0 mm
#define position1 (303616/25 * 100)    //Position � 100 mm
#define position2 (303616/25 * 250)    //Position � 250 mm
#define position3 (303616/25 * 400)    //Position � 400 mm

SSPOSSPEED vitesse0,vitesse1,vitesse2,vitesse3,vitesse4; //variables de vitesse
SSPOSRAMP posRampe0,posRampe1,posRampe2,posRampe3,posRampe4; //variables d'acc�l�ration

/*=============================================
Fonction principale
===============================================*/

main()
{

// ----- Creation des vitesses -----

  vitesse0.CW = 4400*10;      // 4400r/min en marche avant
  vitesse0.CCW = 4400*10;     // 4400r/min en marche arri�re

  vitesse1.CW = 1200*10;      // 1200r/min en marche avant
  vitesse1.CCW = 1200*10;     //1200r/min en marche arri�re

  vitesse2.CW = 3000*10;       //3000r/min en marche avant
  vitesse2.CCW = 3000*10;      //3000r/min en marche arri�re

  vitesse3.CW = 700*10;         //700r/min en marche avant
  vitesse3.CCW = 700*10;        //700r/min en marche arri�re

  vitesse4.CW = 1500*10;         //1500r/min en marche avant
  vitesse4.CCW = 1500*10;         //1500r/min en marche arr�re


// ----- Creation des rampes -----

  posRampe0.Up = 12000;       //Acc�l�ration de 12s
  posRampe0.Down = 12000;     //D�cc�l�ration de 12s

  posRampe1.Up = 10000;          //Acc�l�ration de 10s
  posRampe1.Down = 10000;        //D�cc�l�ration de 10s

  posRampe2.Up = 7000;          //Acc�l�ration de 7s
  posRampe2.Down = 7000;       //D�cc�l�ration de 7s

  posRampe3.Up = 3000;          //Acc�l�ration de 3s
  posRampe3.Down = 3000;       //D�cc�l�ration de 3s

  posRampe4.Up = 5000;          //Acc�l�ration de 5s
  posRampe4.Down = 5000;       //D�cc�l�ration de 5s


//-------D�claration de la t�che 2 -----

  _SetTask2(T2_START, Process); //On d�marre en parall�le une t�che pour le process


// ------Boucle principale -----

  while(1)
  {
    Verrouillage(); //On d�marre la t�che parall�le de verrouillage
  }
}

/*==========================================
Sous-fonctions
===========================================*/

Process()
{
    _Go0(GO0_C_NW_CAM); //r�f�rencement par c�me

    _SetSys(SS_POSRAMP, posRampe0) ;  // Choix de la rampe
    _SetSys(SS_POSSPEED, vitesse0) ; // Choix de la vitesse
    _GoAbs( GO_WAIT, position3 ) ;   //Positionnement absolu sans attente

    _Wait(3000); //pause de 3s

    _SetSys(SS_POSRAMP, posRampe1) ;
    _SetSys(SS_POSSPEED, vitesse1) ;
    _GoAbs( GO_WAIT, position0 ) ;

    _Wait(3000);

    _SetSys(SS_POSRAMP, posRampe2) ;
    _SetSys(SS_POSSPEED, vitesse2) ;
    _GoAbs( GO_WAIT, position3 ) ;

    _Wait(3000);

    _SetSys(SS_POSRAMP, posRampe3) ;
    _SetSys(SS_POSSPEED, vitesse3) ;
    _GoAbs( GO_WAIT, position2 ) ;

    _Wait(3000);

    _SetSys(SS_POSRAMP, posRampe3) ;
    _SetSys(SS_POSSPEED, vitesse4) ;
    _GoAbs( GO_WAIT, position1 ) ;

    _Wait(3000);

    _SetSys(SS_POSRAMP, posRampe4) ;
    _SetSys(SS_POSSPEED, vitesse2) ;
    _GoAbs( GO_WAIT, position0 ) ;
}

/*===================================
   T�che 2 en parall�le
====================================*/

Verrouillage()
{
  if (!DI12) {
   SortieDO10_0;   //vert �teint
   _AxisStop(AS_RSTOP); //arret rapide => blocage de l'axe sans prendre en compte la rampe
   SortieDO11_1;   //rouge allum�
  } else {
   SortieDO11_0;    // rouge �teint
   _AxisStop(AS_ENABLE); // d�blocage de l'axe
   SortieDO10_1;   //vert allum�
  }

  if (DI13) {
   _SetTask2(T2_STOP, Process);  //Stop de la
   SortieDO12_1;  //blanc allum�
   _AxisStop(AS_ENABLE);
   _SetSys(SS_POSRAMP, posRampe4) ;
   _SetSys(SS_POSSPEED, vitesse2) ;
   _GoAbs( GO_WAIT, position0 ) ;
   _SetTask2(T2_START, Process);
   SortieDO12_0; //blanc �teint
  }
}
                                                                         