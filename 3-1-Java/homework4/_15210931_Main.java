package homework4;
import java.util.*;

public class _15210931_Ф����_4_Main {
	public static void main(String[] args) {
		_15210931_Ф����_4_Factory f = new _15210931_Ф����_4_Factory();
		f.initEmpoyees();
		
		Scanner sc = new Scanner(System.in);
		int state = 0;
		int temp_int;
		String temp_str;
		double temp_d;
		_15210931_Ф����_4_Employee temp_emp;
		while (true) {
			System.out.println();
			System.out.println("�����б�");
			System.out.println("1. ��ѯԱ��");
			System.out.println("2. ����Ա��");
			System.out.println("3. ɾ��Ա��");
			System.out.println("4. ��ӡȫ��Ա����Ϣ");
			System.out.print("���������Ĳ�����");
			temp_int = sc.nextInt();
			if (temp_int <= 8 && temp_int >= 0) {
				state = temp_int;
			}
			else {
				continue;
			}
			sc.nextLine();
			switch (state) {
			case 1:
				System.out.print("������Ա���籣�ţ�");
				temp_str = sc.nextLine();
				temp_emp = f.getEmployee(temp_str);
				if (temp_emp == null)
					System.out.println("Ա�������ڣ���ѯʧ��");
				else 
					System.out.print(temp_emp.toString());
				break;
			
			case 2:
				System.out.print("������Ա���籣�ţ�");
				temp_str = sc.nextLine();
				temp_emp = f.getEmployee(temp_str);
				if (temp_emp == null)
					System.out.println("Ա�������ڣ�����ʧ��");
				else {
					System.out.println("�����б�");
					System.out.println("1. ������");
					System.out.println("2. ������");
					System.out.print("���������Ĳ�����");
					temp_int = sc.nextInt();
					temp_str = sc.nextLine();
					if (temp_int == 1) {
						System.out.print("�������µ��գ�");
						temp_str = sc.nextLine();
						temp_emp.setLastName(temp_str);
					}
					else if (temp_int == 2) {
						System.out.print("�������µ����֣�");
						temp_str = sc.nextLine();
						temp_emp.setLastName(temp_str);
					}
					else
						System.out.println("�������");
				}
				break;
				
			case 3:
				System.out.print("������Ա���籣�ţ�");
				temp_str = sc.nextLine();
				temp_emp = f.deleteEmployee(temp_str);
				if (temp_emp == null)
					System.out.println("Ա�������ڣ�ɾ��ʧ��");
				else {
					System.out.println("��ɾ������Ա����");
					System.out.print(temp_emp.toString());
				}
				break;
				
			case 4:
				f.printEmployees();
				break;
				
			default:
				break;
			}
		}
	}
}
