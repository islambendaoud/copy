				TP2: API Rest

DOUAILLE Erwan
DOUYLLIEZ Maxime

============
Introduction
============

	Nous avons créer une API Rest. Elle implémente l'ensemble des verbes HTML, GET POST PUT et DELETE.
Il est donc possible de télécharger, téléverser et supprimer un fichier sur le serveur FTP ainsi que de parcourir la liste des fichiers/dossiers.
L'API renvoie principalement du html, mais le JSON à également était implémenté pour lister les fichiers.

========
Executer
========

 	Placer le .war fournit dans l'archive dans webapps directory des serveurs tomcat/glassfish... Lancer les serveurs, et aller a : http://localhost:8080/RestGateway/


============
Architecture
============
Package:
--------

restGateWay

Classes:
--------


-----------------------
RestGateWay
RestGateWay sert d'interface entre l'utilisateur, le serveur FTP et le générateur d'html. Il intercepte et traite les requetes REST du client, en lui répondant sous un format intélligible. Son role est de rendre les interactions transparentes entre le serveur FTP et l'utilisateur.


-----------------------
HTMLGenerator

HTMLGenerator permet de générer des pages HTML valides pour un navigateur, Elle crée au besoin des formulaires et inclus les verbes POST, DELETE, GET et PUT pour faciliter la navigation de l'utilisateur et génerer de nouvelles requetes REST.




-----------------------
NameStorageBean
NameStorageBean facilite l'interaction avec le serveur Ftp en stockant les informations de l'utilisateur dans une session.


-----------------------
FTPFileEncapsulator

Classe héritant de FTPFile, lui ajoutant une chaine de caractere en attribut afin de retourner un message d'erreur en JSON si besoin est, la méthode "getFileListJSON" devant retourner un tableau de FTPFile parsé en JSON.



-----------------------
ApplicationConfig

Autorise l'utilisation de la librarie jersey en indiquant le chemin d'application et permet de ce fait d'utiliser les annotations @FormDataParam, utilisé lors du maping du téléversement. 


=============
Design Pattern
=============

Singleton


=======================
Qualité du code: design
=======================
Nous n'avons pas utilisé d'interface.
Nous n'avons pas utiliséde méthodes abstraite.
Nous avons utilisé de l'heritage pour ajouter une chaine de caractere afin de gérer une erreur plus élégament(FTPFileEncapsulator).
Nous avons utilisé les différentes annotations permetant d'intercepter les requetes REST:
	-@Path()
	-@Produces()
	-@FormParam()
	-@FormDataParam()
	-@Consumes()
	-@GET
	-@POST
	-@PUT
	-@DELETE
Les annotations usuelles de JUnit ont été utilisées.
	


============
Test Junit
============
Nous avons rencontré un problème lors de l'élaboration des test JUnits pour notre passerelle. Les test JUnits renvoyent dans une grande majorité des cas des pages html contenant le path, ou des noms de ressources. Nous avons utilisé un serveur FTP lancé depuis un jar. Ce serveur browse les ressources depuis la racine. Lors de la comparaison entre le resultat attendu et le resultat obtenu , le test echouera systématiquement ( dans une grande majorité des cas) car les tests on été conçu sur une machine différente avec une arborescence de fichiers différente.

Les classes de test on tout de même été générés.

De plus afin de faciler la lisibilité des classes de test, les résultats attendus sont obtenues via l'appel à des methodes statiques de la classe JUnitTestString. On évite ainsi de créer des classes de test contenant des pages html de plus de 150 lignes.

===============
Gestion d'erreur 
===============

Dans la plupart de nos méthodes de la passerelle REST, RestGateWay, nous nous connectons au serveur et nous logons. A chaque fois nou fesons un try/catch et retournons un HTML adapté à la situation, l'erreur est affiché et une redirection s'effectue pour rammener l'utilisateur dans le comportement voulue.

Pour chaque erreur nous avons anticipé les erreurs. Le contenu HTML est systematiquement renvoyé.
============
Code Samples
============

Permet le téléchargement d'un fichier. l'utilisateur peut saisir directement l'adresse de la ressource (une fois identifié)
---------------------------------------------------------------------------------------------------------------------------

    /**
     * GetFile start the download of fileName parameter include in dir directory
     *
     * @param dir
     * @param fileName
     * @return
     */
 @GET
    @Path("/file/{var: .*}/download/{fileName}")
    @Produces("application/octet-stream")
    public Response getFile(@PathParam("var") String dir, @PathParam("fileName") String fileName) {
        FTPClient client = new FTPClient();
        try {
            client.connect("127.0.0.1", 2121);
        } catch (IOException ex) {
            return null;
        }
        try {
            if (client.login(this.nameStorage.getLogin(), this.nameStorage.getPassword())) {
                client.cwd(dir);
                InputStream in = client.retrieveFileStream(fileName);
                Response reponse = Response.ok(in, MediaType.APPLICATION_OCTET_STREAM).build();
                client.disconnect();
                return reponse;
            }
        } catch (IOException ex) {
            return null;
        }
        return null;
    }







Permet de téléverser grace au verbe POST un fichier spécifié par l'utilisateur dans un formulaire.
----------------------------------------------------------------------------------------------------------------
   /**
     * This is the POST methode than provide you to upload files, param file is
     * the selected file to upload, dir is the directory where file has to be
     * uploaded
     *
     * @param file
     * @param fileDetail
     * @param dir
     * @return
     */
    @POST
    @Path("/file/{dir: .*}")
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces("text/html")
    public String upload(@FormDataParam("file") InputStream file, @FormDataParam("file") FormDataContentDisposition fileDetail, @PathParam("dir") String dir) {
        FTPClient client = new FTPClient();
        try {
            client.connect("127.0.0.1", 2121);
        } catch (IOException ex) {
            return HTMLGenerator.getInstance().getBadConnectionContent();
        }
        try {
            if (!client.login(this.nameStorage.getLogin(), this.nameStorage.getPassword())) {
                return HTMLGenerator.getInstance().getBadConnectionContent();
            } else {
                client.cwd(dir);
                System.out.println(file);
                System.out.println(fileDetail);
                client.storeFile(fileDetail.getFileName(), file);
                client.disconnect();
                return HTMLGenerator.getInstance().getUploadConfirmation(dir, fileDetail.getFileName());
            }
        } catch (IOException e) {
            return HTMLGenerator.getInstance().getSomethingWentWrong(e.getMessage());
        }
    }













Permet de d'obtenir la liste des ressources présentes dans le dossier courrant du serveur FTP et renvoi à l'utilisateur une page html la liste formatée
------------------------------------------------------------------------------------------------------------------------------------------------------
/**
     * GetFileList return the file list directory of the server using dir
     * parameter as the ftp server working directory
     *
     * @param dir
     * @return
     */
    @GET
    @Path("/file/{var: .*}")
    @Produces("text/html")
    public String getFileList(@PathParam("var") String dir) {
        FTPClient client = new FTPClient();
        try {
            client.connect("127.0.0.1", 2121);
        } catch (IOException ex) {
            return HTMLGenerator.getInstance().getBadConnectionContent();
        }
        try {
            if (client.login(this.nameStorage.getLogin(), this.nameStorage.getPassword())) {
                client.cwd(dir);
                FTPFile[] fileList = client.listFiles();
                String cwd = client.printWorkingDirectory();
                client.disconnect();
                return HTMLGenerator.getInstance().getFileListWith(cwd, fileList);
            }
        } catch (IOException ex) {
            return HTMLGenerator.getInstance().getSomethingWentWrong(ex.getMessage());
        }
        return "";
    }
    


Améliore la lisibilité des classes de test en stockant la pages html dans la classe JUnitTestString. Voici un exemple avec une chaine de petite taille. Imaginer avec une String de 400 lignes dans un test JUnit qui en fait 15
---------------------------------------------------------------------------------------------------
 /**
     * Return the expected result of getSignInContent method Test, of class HTMLGenerator.
     */
 
    public static String testGetSignInContent() {
      
    String result=null;
    result="<html><meta http-equiv=\"refresh\" content=\"1; URL=file\"><body><h1>Login ...</h1></body></html>";
    return result; }
