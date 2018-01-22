
package homework7;

import java.util.ArrayList;

import javax.swing.JDialog;
import javax.swing.JLabel;

public class _15210931_肖蕴明_7_BookCatalog {
	public static String[] categoryStrings = new String[] {
		"非教材类的计算机图书", "教材类图书",
		"连环画图书", "养生类图书","其他"};

	private ArrayList<_15210931_肖蕴明_7_BookSpecification> books;

	public _15210931_肖蕴明_7_BookCatalog() {
		books = new ArrayList<_15210931_肖蕴明_7_BookSpecification>();
	}

	public ArrayList<_15210931_肖蕴明_7_BookSpecification> getBooks() {
		return books;
	}

	public void addBook(_15210931_肖蕴明_7_BookSpecification newBook) throws Exception {
		if (newBook.getType() > categoryStrings.length || newBook.getType() < 1) {
			_15210931_肖蕴明_7_ErrorDlg log = new _15210931_肖蕴明_7_ErrorDlg("输入错误");
	        log.setVisible(true);
		}

		for (_15210931_肖蕴明_7_BookSpecification book : books)
			if (book.getISBN().equals(newBook.getISBN())) {
				_15210931_肖蕴明_7_ErrorDlg log = new _15210931_肖蕴明_7_ErrorDlg("ISBN 错误");
		        log.setVisible(true);
			}
		books.add(newBook);
	}

	public _15210931_肖蕴明_7_BookSpecification getBook(String isbn) throws Exception {
		for (_15210931_肖蕴明_7_BookSpecification book : books)
			if (book.getISBN().equals(isbn))
				return book;
		_15210931_肖蕴明_7_ErrorDlg log = new _15210931_肖蕴明_7_ErrorDlg("ISBN 错误");
        log.setVisible(true);
        return null;
	}
}