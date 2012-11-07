using PivotalTracker;
using System;
using System.IO;
using System.Text;

namespace PivotalTrackerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.BackgroundColor = ConsoleColor.Black;
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("Name: ");
            Console.ForegroundColor = ConsoleColor.White;
            string name = Console.ReadLine();
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("Password: ");
            Console.ForegroundColor = ConsoleColor.White;
            string password = Console.ReadLine();
            Console.ForegroundColor = ConsoleColor.Green;
            UserAuthentication au = new UserAuthentication(name, password);
            Project project = new Project();
            project.ID = 679831;
            project.Refresh(au);
        }
    }
}
