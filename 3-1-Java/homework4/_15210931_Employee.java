package homework4;

public abstract class _15210931_Ф����_4_Employee {
	String firstName;
	String lastName;
	String socialSecurityNumber;
	
	_15210931_Ф����_4_Employee(String fn, String ln, String ssn) {
		this.firstName = fn;
		this.lastName = ln;
		this.socialSecurityNumber = ssn;
	}
	
	public abstract double earning();
	
	public String getFirstName() {
		return this.firstName;
	}
	
	public void setFirstName(String newName) {
		this.firstName = newName;
	}

	public String getLastName() {
		return this.lastName;
	}
	
	public void setLastName(String newName) {
		this.lastName = newName;
	}
	
	public String getSocialSecurityNumber() {
		return this.socialSecurityNumber;
	}
	
	public String toString(){
        return	"����" + getFirstName() + '\n' +
                "�գ�" + getLastName() + '\n' +
                "�籣���ţ�" + getSocialSecurityNumber() + '\n' +
                "ÿ��нˮ��" + String.valueOf(earning()) + '\n';
    }
}
