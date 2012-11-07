using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Xml;

namespace PivotalTracker
{
    public class UserAuthentication
    {
        public readonly string UserName;
        public readonly string Token;
        private string password;

        public List<Project> Projects = new List<Project>();
        private Project currentProject;
        public Project CurrentProject 
        {
            get
            {
                return currentProject;
            }
            set
            {
                if (Projects.Contains(value))
                {
                    currentProject = value;
                }
            }
        }

        public UserAuthentication(string userName, string password)
        {
            UserName = userName;
            this.password = password;
            Token = GetTokenByData(UserName, password);
        }

        public UserAuthentication(string token)
        {
            Token = token;
        }

        private string GetTokenByData(string userName, string password)
        {
            string rawToken = GetRawTokenByData(userName, password);
            using (XmlReader reader = XmlReader.Create(new StringReader(rawToken)))
            {
                bool isTokenLastElement = false;
                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element :
                            isTokenLastElement = (reader.Name == "guid");
                            break;
                        case XmlNodeType.Text :
                            if (isTokenLastElement)
                                return reader.Value;
                            break;
                        default :
                            break;
                    }
                }
            }
            return null;
        }

        private string GetRawTokenByData(string userName, string password)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create("https://www.pivotaltracker.com/services/v3/tokens/active/");
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            string data = "username=" + userName + "&" + "password=" + password;
            byte[] datasAsBytes = Encoding.ASCII.GetBytes(data);
            request.ContentLength = datasAsBytes.Length;
            using (Stream stream = request.GetRequestStream())
            {
                stream.Write(datasAsBytes, 0, datasAsBytes.Length);
                stream.Close();
            }
            StringBuilder stringBuilder = new StringBuilder();
            using (StreamReader streamReader = new StreamReader(request.GetResponse().GetResponseStream()))
            {
                string streamLine = "";
                while (true)
                {
                    streamLine = streamReader.ReadLine();
                    if (!String.IsNullOrEmpty(streamLine))
                        stringBuilder.AppendLine(streamLine);
                    else
                        break;
                }
            }
            return stringBuilder.ToString();
        }
    }
}
