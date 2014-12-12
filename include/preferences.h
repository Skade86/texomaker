#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtWidgets>
#include <QObject>
#include <QMap>

//! Classe de préférences
/*! 
 Classe un peu particulière, contenant uniquement des attributs privés statiques qui stocke un certain nombre de paramètres
 dont beaucoup d'objets auront besoin, et auquels ils pourront accéder par les méthodes publiques correspondantes.
 */
class Preferences
	{
	private:
		
		//! Emplacement du fichier de base de donnée
		static QString dbfile;
		
		//! Contenu du préambule LaTeX
		static QString preamble;
		
		//! Inclusion de macros perso
		static QString macroFiles;
		
		//! Compilateur utilisé : latex ou pdflatex
		static QString compiler;
		
		//! Emplacement du script pour latex+dvips+ps2pdf
		static QString ltx2pdf;
		
		//! Tableau associatif associant pdflatex, latex, dvipsd, ps2pdf, mpost à leurs emplacement
		static QMap<QString, QString> binMap;
		
		//! Liste des titres de métadonnées
		static QStringList metaList;
		
		//! Liste des metadonnées à afficher
		static QStringList metaToView;
		
		//! Entête pour les feuilles d'exercices
		static QString sheetHeader;
		
		//! Macro d'exo pour les feuilles d'exercices
		static QString sheetExo;
		
		//! Intervale entre deux exercices
		static QString sheetBetween;
		
		//! Pied pour les feuilles d'exercices
		static QString sheetFooter;
		
		//! Ce qui commence le contenu (\begin{document}, ...)
		static QString beginDoc;
		
		//! Ce qui finit le contenu (\begin{document}, ...)
		static QString endDoc;
		
        //! Option au démarrage
        static bool openAtLaunch;

        //! Option de compilation
        static QString compilationOptions;

        //! Liste des dossiers favoris de feuilles d'exos
        static QStringList sheetBookmarks;

        static bool useIso;
		
	public:
		
		//! Constructeur vide (aucun objet n'aura besoin d'être créé)
		Preferences() { };
		
		//! Version actuelle
		static  QString curVersion;

        //! Récupère useIso
        static bool p_getUseIso() { return useIso; }

        //! Fixe useIso
        static void p_setUseIso(bool iso) { useIso=iso; }
		
		//! Emplacement de la vase d'échange SqLite
		static QString sqliteDbPath;
		
		//! Récupère le contenu de preamble
		static QString p_getPreamble() { return preamble; }
		
		//! Donne la valeur str à preamble
		static void p_setPreamble(QString str) { preamble=str;	}
		
		//! Récupère la valeur du binaire bin
		static QString p_getBin(QString bin) { return binMap.value(bin);	}
		
		//! Donne la valeur valStr au binaire keyStr
		static void p_setBin(QString keyStr,QString valStr) { binMap[keyStr]=valStr; }
		
		//! Récupère la valeur de dbfile
		static QString p_getDbfile() { return dbfile; }
		
		//! Donne la valeur str à dbfile
		static void p_setDbfile(QString str) { dbfile=str;	}
		
		//! Récupère la valeur de ltx2pdf
		static QString p_getLtx2pdf() { return ltx2pdf; }
		
		//! Donne la valeur str à ltx2pdf
		static void p_setLtx2pdf(QString str) { ltx2pdf=str;	}
		
		//! Récupère la valeur de compiler
		static QString p_getCompiler() { return compiler; }
		
		//! Donne la valeur str à compiler
		static void p_setCompiler(QString str) { compiler=str;	}
		
		//! Récupère la valeur de metaList
		static QStringList p_getMetaList() { return metaList; }
		
		//! Donne la valeur str à metaList
		static void p_setMetaList(QStringList strLst) { metaList=strLst;	}
		
		//! Retourne le nombre de métadonnées
		static int p_getMetaSize() { return metaList.size(); }
		
		//! Récupère la valeur de metaToView
		static QStringList p_getMetaToView() { return metaToView; }
		
		//! Donne la valeur str à metaToView
		static void p_setMetaToView(QStringList strLst) { metaToView=strLst;	}
		
		//! Récupère la valeur de sheetHeader
		static QString p_getSheetHeader() { return sheetHeader; }
		
		//! Donne la valeur str à sheetHeader
		static void p_setSheetHeader(QString str) { sheetHeader=str;	}
		
		//! Récupère la valeur de sheetFooter
		static QString p_getSheetFooter() { return sheetFooter; }
		
		//! Donne la valeur str à sheetFooter
		static void p_setSheetFooter(QString str) { sheetFooter=str;	}
		
		//! Récupère la valeur de sheetExo
		static QString p_getSheetExo() { return sheetExo; }
		
		//! Donne la valeur str à sheetExo
		static void p_setSheetExo(QString str) { sheetExo=str;	}
		
		//! Récupère la valeur de sheetBetween
		static QString p_getSheetBetween() { return sheetBetween; }
		
		//! Donne la valeur str à sheetBetween
		static void p_setSheetBetween(QString str) { sheetBetween=str;	}
		
		//! Récupère la valeur de beginDoc
		static QString p_getBeginDoc() { return beginDoc; }
		
		//! Donne la valeur str à beginDoc
		static void p_setBeginDoc(QString str) { beginDoc=str;	}
		
		//! Récupère la valeur de endDoc
		static QString p_getEndDoc() { return endDoc; }
		
		//! Donne la valeur str à endDoc
		static void p_setEndDoc(QString str) { endDoc=str;	}

        //! Récupère la valeur de openAtLaunch
        static bool p_getOpenAtLaunch() { return openAtLaunch; }

        //! Donne la valeur openAtLaunch
        static void p_setOpenAtLaunch(bool logic) { openAtLaunch=logic; }
		
		//! Récupère la valeur de compilationOptions
		static QString p_getCompilationOptions() { return compilationOptions; }
		
		//! Donne la valeur str à compilationOptions
		static void p_setCompilationOptions(QString str) { compilationOptions=str;	}
		
		//! Récupère la valeur de macroFiles
		static QString p_getMacroFiles() { return macroFiles; }
		
		//! Donne la valeur str à macroFiles
		static void p_setMacroFiles(QString str) { macroFiles=str;	}

        //! Récupère la valeur de sheetBookmarks
        static QStringList p_getSheetBookmarks() { return sheetBookmarks; }

        //! Donne la valeur str à sheetBookmarks
        static void p_setSheetBookmarks(QStringList strLst) { sheetBookmarks=strLst;	}
	};
#endif
