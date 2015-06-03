all:
	make -C ./Modele_CS
	make -C ./Modele_D


clear:
	make -C ./Modele_CS clear
	make -C ./Modele_D clear 


clean:
	make -C ./Modele_CS clean
	make -C ./Modele_D clean
