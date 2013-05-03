#################################################
# TUTORIAL Computree - FR - Version du 03/05/2013
# A. PIBOULE, ONF - alexandre.piboule@onf.fr
#
# Ce fichier (_LISEZ_MOI_.pri) est le point de d�part de ce tutoriel.
# Ses lignes sont comment�es (#) afin de ne pas perturber le projet Qt.
# Lors de la lecture il faut donc ignorer les signes #
#
# Ce projet est un plugin fonctionnel pour computree
# Il contient des �tapes visant � expliquer les m�canismes de cr�ation d'un plugin
# Chaque �tape est comment�e
#
#########################
# Structure d'un plugin :
#
# Chaque plugin est par convention identifi� par � un code de 2 � 4 lettres majuscules
# qui pr�fixent les noms de toutes les classes du plugin.
# Le pr�sent plugin a pour code TUFR (comme TUtorial version FRan�aise)
#
# Le fichier plugintutorialfr.pro est le fichier "projet Qt".
# Ainsi par convention le fichier "projet Qt" d'un plugin est nomm� pluginnomduplugin.pro
# C'est lui qui permet � Qt-Creator de conna�tre les fichiers du projet.
# Pour plus d'information sur les fichiers .pro, voir la documentation de Qt
#
# Un plugin Computree doit contenir en d�but de son fichier .pro les deux lignes suivantes :
# (sans le signe #)
# include(../../pluginsharedv2/shared.pri)
# include($${PLUGIN_SHARED_DIR}/include.pri)
#
# Cela permet d'inclure dans le projet les �l�ments de Computree n�cessaires
# La cons�quence, est la disponibilit� des fichiers suivants dans le projet Qt :
# - destdir.pri
# - include.pri
# - shared.pri
#
# ATTENTION : ces trois fichiers ne doivent en aucun cas �tre modifi�s !!!
#
# Ensuite le fichier .pro doit contenir la directive suivante :
# (sans le signe #)
# TARGET = plug_nomDuPlugin
# Cela d�finira le nom de la librairie du plugin une fois compil�
# Ici le nom du plugin est tutorialfr, ce qui donne : TARGET = plug_tutorialfr
#
# Pour fonctionner un plugin Computree doit contenir (au moins) deux classes :
# - CODE_PluginEntry (d�finie dans tufr_pluginentry.h et tufr_pluginentry.cpp ici)
# - CODE_StepPluginManager (d�finie dans tufr_steppluginmanager.h et tufr_steppluginmanager.cpp ici)
#
# La classe CODE_PluginEntry permet � Computree d'utiliser le plugin
# La classe CODE_StepPluginManager d�finit les �tapes du plugin utilisables
#
# Ensuite on ajoute une classe pour chaque �tape ou item du plugin,
# respectivement dans des repertoires step et itemdrawable
# Dans le cas g�n�ral la cr�ation d'item n'est pas utile �tant donn�
# qu'il est pr�f�rable de n'utiliser que des items standard d�finis dans PluginSharedV2
# Ainsi, la plupart du temps un plugin ne contient que des �tapes.
#
##########################################################
# Ordre pr�conis� pour la lecture des fichiers de ce tutorial :
# 1) En_t�tes : tufr_pluginentry.h
# 2) Sources  : tufr_pluginentry.cpp
#
# 3) En_t�tes : tufr_steppluginmanager.h
# 4) Sources  : tufr_steppluginmanager.cpp
#
# 5) En_t�tes : step/tufr_steptutorial01.h
# 8) Sources  : step/tufr_steptutorial01.cpp
