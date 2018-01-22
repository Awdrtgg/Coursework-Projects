import java.util.Scanner;

public class StudentListTest {
	public static void main(String []args) {

		Scanner sc = new Scanner(System.in);
		
		StudentList stulist = new StudentList(10);
		
		int state = 0;
		int temp_int;
		String temp_str;
		Student temp_stu;
		while (true) {
			System.out.print("请输入您的操作：");
			temp_int = sc.nextInt();
			if (temp_int <= 8 && temp_int >= 0) {
				state = temp_int;
			}
			sc.nextLine();
			switch (state) {
			case 1:
				String number, name;
				int maths, english, science;
				
				System.out.println("请输入学生信息：");
				System.out.print("学号：");
				number = sc.nextLine();
				System.out.print("姓名：");
				name = sc.nextLine();
				System.out.print("数学成绩：");
				maths = sc.nextInt();
				System.out.print("英语成绩：");
				english = sc.nextInt();
				System.out.print("科学成绩：");
				science = sc.nextInt();
				
				Student ns = new Student(number, name);
				ns.enterMarks(maths, english, science);
				
				stulist.add(ns);
				stulist.printInfo();
				break;
				
			case 2:
				System.out.print("请输入学生的学号：");
				temp_str = sc.nextLine();
				temp_stu = new Student(temp_str, "");
				if (stulist.remove(temp_stu)) {
					System.out.println("删除成功");
					stulist.printInfo();
				}
				else {
					System.out.println("对不起，没有对应的学生");
				}
				break;	
			
			case 3:
				System.out.print("请输入学生的位置：");
				temp_int = sc.nextInt();
				if (stulist.remove(temp_int)) {
					System.out.println("删除成功");
					stulist.printInfo();
				}
				else {
					System.out.println("对不起，没有对应的学生");
				}
				break;
				
			case 4:
				if (stulist.isEmpty()) {
					System.out.println("当前没有学生");
				}
				else {
					System.out.println("当前有学生");
				}
				break;
			
			case 5:
				System.out.print("请输入学生的位置：");
				temp_int = sc.nextInt();
				temp_stu = stulist.getItem(temp_int);
				if (temp_stu != null) {
					System.out.println(temp_stu);
				}
				else {
					System.out.println("对不起，没有对应的学生");
				}
				break;
				
			case 6:
				System.out.print("请输入学生的学号：");
				temp_str = sc.nextLine();
				temp_stu = new Student(temp_str, "");
				temp_stu = stulist.getItem(temp_stu);
				if (temp_stu != null) {
					System.out.println(temp_stu);
				}
				else {
					System.out.println("对不起，没有对应的学生");
				}
				break;
				
			case 7:
				stulist.printInfo();
				break;
				
			case 8:
				state = 8;
				break;
				
			default:
				break;
			}
		if (state == 8) {
			break;
		}
		}
	}
}
