using System;
using System.Text;

namespace PivotalTracker
{
    /// <summary>
    /// A Task subdivides a Story to display details or measure the progress of a bigger one.
    /// </summary>
    public class Task
    {
        /// <summary>
        /// ID of the Task.
        /// </summary>
        public int ID { get; set; }
        /// <summary>
        /// Task's Position.
        /// </summary>
        public int Position { get; set; }
        /// <summary>
        /// Tasks's Description.
        /// </summary>
        public string Description { get; set; }
        /// <summary>
        /// Is the Task completed?
        /// </summary>
        public bool IsComplete { get; set; }
        /// <summary>
        /// The Date of the Tasks's Creation.
        /// </summary>
        public DateTime CreationDate { get; set; }

        /// <summary>
        /// Task's default constructor.
        /// </summary>
        public Task()
        {
        }

        /// <summary>
        /// Tasks copy-constructor.
        /// </summary>
        /// <param name="task"></param>
        public Task(Task task)
        {
            ID = task.ID;
            Position = task.Position;
            Description = task.Description;
            IsComplete = task.IsComplete;
            CreationDate = task.CreationDate;
        }

        public string ToXmlString()
        {
            StringBuilder task = new StringBuilder();
            task.Append("<task>");
            task.Append("<id>" + ID + "</id>");
            if (!String.IsNullOrEmpty(Description))
            {
                task.Append("<description>" + Description + "</description>");
            }
            task.Append("<position>" +  Position.ToString() +"</position>");
            task.Append("<complete>" + IsComplete.ToString() + "</complete>");
            task.Append("created_at" + CreationDate.ToString() + " UTC" + "</created_at>");
            task.Append("</task>");
            return task.ToString();
        }
    }
}
