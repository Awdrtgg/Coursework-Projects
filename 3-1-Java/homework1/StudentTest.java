import java.util.*;

public class StudentTest {
	public static void main(String []args) {
		String number, name, str;
		int maths, english, science;
		Scanner sc = new Scanner(System.in);
		Student s;
		
		while (true) {
			try {
				System.out.print("请输入学生学号：");
				number = sc.nextLine();
				s = new Student(number);
				
				break;
			}
			catch(StudentNumberException e) {
				System.out.print(e.getMessage());
			}
			finally {}
		}
		
		System.out.print("请输入学生姓名：");
		name = sc.nextLine();
		s.addName(name);
		
		while (true) {
			try {
				System.out.print("请输入学生三门课成绩（数学，英语，科学）：");
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
				System.out.println("输入格式不正确，请输入整数");
			}
			finally {}
		}	
	}
}
