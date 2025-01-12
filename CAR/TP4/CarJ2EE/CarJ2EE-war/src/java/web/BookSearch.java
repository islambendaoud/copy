package web;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import ejb.Book;
import ejb.BookFacadeLocal;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import javax.ejb.EJB;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

/**
 *
 * @author erwan
 */
public class BookSearch extends HttpServlet {

    @EJB
    private BookFacadeLocal bookFacade;

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code>
     * methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        HttpSession session = request.getSession(true);
        try {
            if (session.getAttribute("user") == null) {
                this.getServletContext().getRequestDispatcher("/index.jsp").forward(request, response);
            }
            response.setContentType("text/html;charset=UTF-8");
            int date = 0;
            String title = "";
            String author = "";
            if (!request.getParameter("date").matches("\\s*") || !request.getParameter("title").matches("\\s*") || !request.getParameter("author").matches("\\s*")) {
                date = (request.getParameter("date").equals("")) ? 0 : Integer.parseInt(request.getParameter("date"));
            }
            response.setContentType("text/html;charset=UTF-8");
            ServletContext sc = getServletContext();
            RequestDispatcher rd = sc.getRequestDispatcher("/header.jsp");
            rd.include(request, response);
            title = request.getParameter("title");
            author = request.getParameter("author");

            try (PrintWriter out = response.getWriter()) {
                out.println("<section>");
                rd = sc.getRequestDispatcher("/searchBar.jsp");
                rd.include(request, response);
                out.println("<article>");
                List<Book> books = bookFacade.findAll();
                for (Book book : books) {
                    if (book.getAuthor().toLowerCase().contains(author.toLowerCase()) && book.getTitle().toLowerCase().contains(title.toLowerCase()) && (book.getDate() == date || date == 0)) {
                        out.println("<table>");
                        out.println("<tr><td><b>Title: </b></td><td><b>" + book.getTitle() + "</b></td></b></tr>");
                        out.println("<tr><td>Author: </td><td>" + book.getAuthor() + "</td></tr>");
                        out.println("<tr><td>Date: </td><td>" + book.getDate() + "</td></tr>");
                        out.println("<tr><td>Price: </td><td>" + book.getPrice() + "</td></tr>");
                        out.println("</table>");
                        out.println("<form method=POST action=\"AddInCart\"><input type=hidden name=\"title\" value=\"" + book.getTitle() + "\"><input type=\"submit\" value=\"add\"></form>");
                        out.println("<br><br>");
                    }
                }
                out.println("</article>");
                rd = sc.getRequestDispatcher("/cartView.jsp");
                rd.include(request, response);
                out.println("</section>");
                rd = sc.getRequestDispatcher("/foot.jsp");
                rd.include(request, response);
                out.close();
            }
        } catch (Exception e) {
            this.getServletContext().getRequestDispatcher("/index.jsp").forward(request, response);
        }
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Short description";
    }// </editor-fold>

}
