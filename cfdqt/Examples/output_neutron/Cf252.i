single det
c __________________________________________________________                    
c CELLS 
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
c  Source
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c Cs source
   222  8 -0.99 -222 imp:n=1 
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c   water cylinder
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
   230  8 -0.99 -230 222 imp:n=1
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c   detector
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
   300     0      -300     imp:n=1
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c   air
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   900  0  -999 #222 #230 #300 imp:n=1 $air
   999     0              999      imp:n=0 $graveyard                                                                 
c END CELL CARDS

c SURFACES         
c  10        rcc   0.5 0.5 0 0 0 1 0.5
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c  cylinder source
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c PuBe
  222 1 RCC 0 0 1.4478 0 0 5.63372 1.4097 $ inner
c barrel
  230 3 RCC 0 0 0 0 0 52 5 $outer
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c   detector
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   
  300       S  75 75 10 2.54
c observation _______________________        
  999       rpp -200 200 -200 200 -200 200  $observed universe

c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c Data
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c Translations
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TR1 25 25 7 9j
TR3 25 25 0 9j
*TR2 25 25 7 45 -45 90 135 45 90 90 90 0
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c Physics
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mode  n                                                               
nps 1E8   
  FILES 21 DUMN1
  DBCN
  PRDMP 4J -1                                                            
c CTME 590                                                                      
c SOURCE DEFINITION                                                             
 sdef  POS= 25 25 8.4478  PAR=n  CEL=222  ERG=d1                                       
       axs= 0 0 1     EXT=d3   RAD=d2               
SC1 Cf-252 Source, from Phys. Rev. 108, 411
#       SI1       SP1
        H         D   
        0         0
        0.478702  0.1
        0.817756  0.1
        1.15082   0.1
        1.50133   0.1
        1.88769   0.1
        2.33337   0.1
        2.87784   0.1
        3.60501   0.1
        4.77086   0.1
        10        0.1
c                                                                                                                                                 
 si2 0  1.4097                                 $radius of source               
 sp2 -21 1                                     $first power sampling                                                                   
 si3  0 5.63372                                $vertical sampling                     
 sp3 -21 0                                     $first power sampling       
c sdef  POS= 0 0 0  PAR=P  ERG=0.6617       $ Cs-137 point source                                                            
c __________________
c c TALLY
 F4:N  300
 sd4   1
c c4    0 1
 E4    1e-9 99ilog 10.0 
c __________________                                                            
c MATERIALS      
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c  NaI d=-3.6670
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  m1  nlib=70c  plib=04p
      11000 -0.153373
      53000 -0.846627
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c  Wood (southern pine) d=-0.64
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  m2 NLIB=70c PLIB=04p   
      1000 -0.059642
      6000 -0.497018
      7000 -0.004970
      8000 -0.427435
      12000 -0.001988
      16000 -0.004970
      19000 -0.001988
      20000 -0.001988
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c  Polycarbonate (Mat. Compendium PNNL)
c  density = -1.200
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  m3  NLIB=60c PLIB=04p
      1001 -0.055491
      6000 -0.755751
      8016 -0.188758
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c Steel, Stainless 202 d=-7.86
c (Mat. Compendium PNNL)
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 m4   6000 -0.000750
      7014 -0.001250
      14000 -0.005000
      15031 -0.000300
      16000 -0.000150
      24000 -0.180000
      25055 -0.087500
      26000 -0.675050
      28000 -0.050000
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c Air, Dry (near sea level) d=-1.205E-3
c (Mat. Compendium PNNL)
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 m5   NLIB=60C PLIB=04P $ Air (dry, near sea level), dencity 0.001205 g/cm3
      6000 -0.000124
      7014 -0.755268
      8016 -0.231781
      18000.59c -0.012827
      98252 -1E-20      
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c Tantalum d=-16.654
c (Mat. Compendium PNNL)
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 m6   73181 -1.000000
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c PuBe d=-3.586
c (Mat. Compendium PNNL)
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 m7   4009  -0.341
      94239 -0.6153742
      94240 -0.0400672
      94241 -0.0034927
      95241 -0.0000659
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
c water d=-0.99
c ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 m8    1001.70c      -0.111894  $water
       8016.70c      -0.888106 
c concrete, regular (PNNL comp.) mass density = -2.30
 m14 NLIB=70c PLIB=04p
     1001 -0.010000
     8016 -0.532000
     11023 -0.029000
     13027 -0.034000
     14028 -0.337000
     20040 -0.044000
     26056 -0.014000