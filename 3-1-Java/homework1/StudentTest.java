import java.util.*;

public class StudentTest {
	public static void main(String []args) {
		String number, name, str;
		int maths, english, science;
		Scanner sc = new Scanner(System.in);
		Student s;
		
		while (true) {
			try {
				System.out.print("������ѧ��ѧ�ţ�");
				number = sc.nextLine();
				s = new Student(number);
				
				break;
			}
			catch(StudentNumberException e) {
				System.out.print(e.getMessage());
			}
			finally {}
		}
		
		System.out.print("������ѧ��������");
		name = sc.nextLine();
		s.addName(name);
		
		while (true) {
			try {
				System.out.print("������ѧ�����ſγɼ�����ѧ��Ӣ���ѧ����");
				str = sc.nextLine();
				String[] temp = str.split(",");
				maths = Integer.valueOf(temp[0]);
				english = Integer.valueOf(temp[1]);
				science = Integer.valueOf(temp[2]);
				s.enterMarks(maths, english, science);
				
				System.out.println(s);
				break;
			}
			catch(SourceException e) {
				System.out.print(e.getMessage());
			}
			catch(NumberFormatException e) {
				System.out.println("�����ʽ����ȷ������������");
			}
			finally {}
		}	
	}
}
