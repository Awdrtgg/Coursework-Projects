
package homework7;

import java.util.ArrayList;
import java.awt.*;
import javax.swing.*;

public class _15210931_肖蕴明_7_AddBookUI extends JFrame
{
	private _15210931_肖蕴明_7_Controller controller;
	private JPanel panel;

	public _15210931_肖蕴明_7_AddBookUI(_15210931_肖蕴明_7_Controller _controller) {
		this.controller = _controller;
		initUI();
	}

	private void initUI() {
		update();
		setTitle("添加书籍");
		setLocationRelativeTo(null);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
	}

	private void update() {
		ArrayList<_15210931_肖蕴明_7_BookSpecification> books = controller.getBooks();
		String[] categoryStrings = controller.getCategoryStrings();
		
		// Clear previous canvas
		if (this.panel != null)
			this.remove(this.panel);

		this.panel = new JPanel();
		this.panel.setLayout(new BoxLayout(this.panel, BoxLayout.Y_AXIS));

		// Scroll the Pane
		JPanel spanel = new JPanel();
		spanel.setLayout(new GridLayout(books.size() + 1, 4));
		spanel.add(new JLabel("ISBN"));
		spanel.add(new JLabel("书名"));
		spanel.add(new JLabel("单价"));
		spanel.add(new JLabel("类型"));
		for (_15210931_肖蕴明_7_BookSpecification book : books) {
			spanel.add(new JLabel(book.getISBN()));
			spanel.add(new JLabel(book.getTitle()));
			spanel.add(new JLabel("" + book.getPrice()));
			spanel.add(new JLabel(categoryStrings[book.getType() - 1]));
		}
		panel.add(new JScrollPane(spanel));
		
		// Footer
		JPanel fpanel = new JPanel();
		fpanel.setLayout(new GridLayout(5, 2));

		// ISBN
		fpanel.add(new JLabel("ISBN"));
		JTextField tISBN = new JTextField(10);
		fpanel.add(tISBN);

		// Name
		fpanel.add(new JLabel("书名"));
		JTextField tName = new JTextField(10);
		fpanel.add(tName);

		// Price
		fpanel.add(new JLabel("单价"));
		JTextField tPrice = new JTextField(10);
		fpanel.add(tPrice);

		// Category
		fpanel.add(new JLabel("类型"));
		JComboBox<String> cCategory = new JComboBox<String>(categoryStrings);
		fpanel.add(cCategory);

		// Buttons
		JButton bAdd = new JButton("确认");
		bAdd.addActionListener((e) -> {
			try {
				controller.addBook(
					new _15210931_肖蕴明_7_BookSpecification(
						tISBN.getText(),
						tName.getText(),
						Double.parseDouble(tPrice.getText()),
						cCategory.getSelectedIndex() + 1
					)
				);
				update();
			}
			catch (NumberFormatException ee) {
				_15210931_肖蕴明_7_ErrorDlg log = new _15210931_肖蕴明_7_ErrorDlg("输入错误");
                log.setVisible(true);
			}
			catch (Exception ee) {
				System.out.println(ee.getMessage());
			}
		});
		JButton bCancel = new JButton("取消");
		bCancel.addActionListener((e) -> {
			this.dispose();
		});
		fpanel.add(bAdd);
		fpanel.add(bCancel);
		
		panel.add(fpanel);
		
		// Add Panel
		this.add(panel);
		this.pack();
	}
}