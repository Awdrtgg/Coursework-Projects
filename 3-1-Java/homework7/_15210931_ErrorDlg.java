package homework7;

import javax.swing.JDialog;
import javax.swing.JLabel;

public class _15210931_Ð¤ÔÌÃ÷_7_ErrorDlg extends JDialog {
    private static final int W = 600;
    private static final int H = 300;
    
    public _15210931_Ð¤ÔÌÃ÷_7_ErrorDlg(String info) {
        super();
        initUI(info);
    }

    private void initUI(String info) {
        // show error info
        this.add(new JLabel(info, JLabel.CENTER));
        this.pack();

        setTitle("Error");
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        setLocationRelativeTo(getParent());
        setSize(W, H);
        this.setModal(true);
    }
}