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
			System.out.print("���������Ĳ�����");
			temp_int = sc.nextInt();
			if (temp_int <= 8 && temp_int >= 0) {
				state = temp_int;
			}
			sc.nextLine();
			switch (state) {
			case 1:
				String number, name;
				int maths, english, science;
				
				System.out.println("������ѧ����Ϣ��");
				System.out.print("ѧ�ţ�");
				number = sc.nextLine();
				System.out.print("������");
				name = sc.nextLine();
				System.out.print("��ѧ�ɼ���");
				maths = sc.nextInt();
				System.out.print("Ӣ��ɼ���");
				english = sc.nextInt();
				System.out.print("��ѧ�ɼ���");
				science = sc.nextInt();
				
				Student ns = new Student(number, name);
				ns.enterMarks(maths, english, science);
				
				stulist.add(ns);
				stulist.printInfo();
				break;
				
			case 2:
				System.out.print("������ѧ����ѧ�ţ�");
				temp_str = sc.nextLine();
				temp_stu = new Student(temp_str, "");
				if (stulist.remove(temp_stu)) {
					System.out.println("ɾ���ɹ�");
					stulist.printInfo();
				}
				else {
					System.out.println("�Բ���û�ж�Ӧ��ѧ��");
				}
				break;	
			
			case 3:
				System.out.print("������ѧ����λ�ã�");
				temp_int = sc.nextInt();
				if (stulist.remove(temp_int)) {
					System.out.println("ɾ���ɹ�");
					stulist.printInfo();
				}
				else {
					System.out.println("�Բ���û�ж�Ӧ��ѧ��");
				}
				break;
				
			case 4:
				if (stulist.isEmpty()) {
					System.out.println("��ǰû��ѧ��");
				}
				else {
					System.out.println("��ǰ��ѧ��");
				}
				break;
			
			case 5:
				System.out.print("������ѧ����λ�ã�");
				temp_int = sc.nextInt();
				temp_stu = stulist.getItem(temp_int);
				if (temp_stu != null) {
					System.out.println(temp_stu);
				}
				else {
					System.out.println("�Բ���û�ж�Ӧ��ѧ��");
				}
				break;
				
			case 6:
				System.out.print("������ѧ����ѧ�ţ�");
				temp_str = sc.nextLine();
				temp_stu = new Student(temp_str, "");
				temp_stu = stulist.getItem(temp_stu);
				if (temp_stu != null) {
					System.out.println(temp_stu);
				}
				else {
					System.out.println("�Բ���û�ж�Ӧ��ѧ��");
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
